#include "CommandHandler.hpp"

Server &CommandHandler::getServer() const { return m_server; }

CommandHandler::CommandHandler(Server &server) : m_server(server) {}


void CommandHandler::handleCommand_(const std::string &command, ClientHandler *clientHandler, int nbcmd,
                                    const std::string cmd[],
                                    void (CommandHandler::*p[])(ClientHandler *, const std::string &)) {
    std::string cmdParsed = parseCommand(command);
    std::transform(cmdParsed.begin(), cmdParsed.end(), cmdParsed.begin(), toupper);
    for (int i = 0; i < nbcmd; ++i) {
        if (cmdParsed == cmd[i] && p[i] != NULL) {
            (this->*p[i])(clientHandler, command);
            return;
        }
    }
    if (!clientHandler->isAuthenticated())
        MessageHandler::sendErrorNoAuth(clientHandler, cmdParsed);
}


void CommandHandler::handleCommandNoRegister(const std::string &command, ClientHandler *clientHandler) {
    const int nbacceptableCmdNoRegister = 5;
    const std::string acceptableCmdNoRegister[nbacceptableCmdNoRegister] = {"NICK", "USER", "USERHOST", "PASS",
                                                                            "WHOIS"};
    void (CommandHandler::*p[nbacceptableCmdNoRegister])(ClientHandler *, const std::string &) = {
            &CommandHandler::handleNick, &CommandHandler::handleUser, &CommandHandler::handleUser,
            &CommandHandler::handlePass, &CommandHandler::handleWhois};
    CommandHandler::handleCommand_(command, clientHandler, nbacceptableCmdNoRegister, acceptableCmdNoRegister, p);
}

void CommandHandler::handleCommandRegister(const std::string &command, ClientHandler *clientHandler) {
    const int nbacceptableCmd = 18;
    const std::string acceptableCmd[nbacceptableCmd] = {"CAP",    "NICK",  "USER", "USERHOST", "PASS",    "MODE",
                                                        "WHOIS",  "JOIN",  "PART", "QUIT",     "PRIVMSG", "NOTICE",
                                                        "INVITE", "TOPIC", "PING", "KICK",     "NAME",    "LIST"};
    void (CommandHandler::*p[nbacceptableCmd])(ClientHandler *, const std::string &) = {
            &CommandHandler::handleCap,    &CommandHandler::handleNick,        &CommandHandler::handleUser,
            &CommandHandler::handleUser,   &CommandHandler::handlePass,        &CommandHandler::handleMode,
            &CommandHandler::handleWhois,  &CommandHandler::handleJoinChannel, &CommandHandler::handlePart,
            &CommandHandler::handleQuit,   &CommandHandler::handlePrivMsg,     &CommandHandler::handlePrivMsg,
            &CommandHandler::handleInvite, &CommandHandler::handleTopic,       &CommandHandler::handlePing,
            &CommandHandler::handleKick,   &CommandHandler::handleName,        &CommandHandler::handleList};
    CommandHandler::handleCommand_(command, clientHandler, nbacceptableCmd, acceptableCmd, p);
}
