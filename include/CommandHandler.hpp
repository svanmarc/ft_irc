#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "ClientHandler.hpp"
#include "IRCConstants.hpp"
#include "Irc.hpp"
#include "MessageHandler.hpp"
#include "Server.hpp"


class CommandHandler {
public:
    // CommandHandler.cpp
    CommandHandler(Server &server);
    void handleCommand_(const std::string &command, ClientHandler *clientHandler, int nbcmd, const std::string cmd[],
                        void (CommandHandler::*p[])(ClientHandler *, const std::string &));
    void handleCommandNoRegister(const std::string &command, ClientHandler *clientHandler);
    void handleCommandRegister(const std::string &command, ClientHandler *clientHandler);

private:
    Server &m_server;
    static const int RPL_LISTSTART = 321;
    static const int RPL_LIST = 322;
    static const int RPL_LISTEND = 323;
    static const int RPL_WHOISSERVER = 312;
    static const int RPL_ENDOFWHOIS = 318;
    static const int RPL_CAPLS = 375;
    static const int RPL_CHANNELMODEIS = 324;
    static const int ERR_UNKNOWNCOMMAND = 421;
    static const int RPL_WHOISUSER = 311;
    static const int ERR_NEEDMOREPARAMS = 461;
    static const int ERR_ALREADYREGISTERED = 462;


    // CommandWhois.cpp
    void handleUser(ClientHandler *clientHandler, const std::string &command);
    void handlePing(ClientHandler *clientHandler, const std::string &command);
    void handlePass(ClientHandler *clientHandler, const std::string &command);
    void completeRegistration(ClientHandler *clientHandler);

    // CommandHandler.cpp
    Server &getServer() const;

    // CommandsUsers.cpp
    void handleWhois(ClientHandler *clientHandler, const std::string &command);
    std::string parseCommand(const std::string &fullCommand);

    // CommandNick.cpp
    bool checkNickname(const std::string &nickname, ClientHandler *clientHandler);
    void handleNick(ClientHandler *clientHandler, const std::string &command);

    // CommandPrivMsg
    void handlePrivMsg(ClientHandler *clientHandler, const std::string &command);

    // CommandJoinPart.cpp
    void handleJoinChannel(ClientHandler *clientHandler, const std::string &command);
    void handlePart(ClientHandler *clientHandler, const std::string &command);

    // CommandMode.cpp
    void handleMode(ClientHandler *clientHandler, const std::string &command);
    void channelModelHandler(ClientHandler *clientHandler, Channel &channel, const std::string &mode,
                             const std::string &param);
    void userModeHandler(ClientHandler *clientHandler, const std::string &mode);

    // CommandQuit.cpp
    void handleQuit(ClientHandler *clientHandler, const std::string &command);
    void handleCap(ClientHandler *clientHandler, const std::string &command);

    void handleInvite(ClientHandler *clientHandler, const std::string &command);
    void handleKick(ClientHandler *clientHandler, const std::string &command);
    void handleOpMode(ClientHandler *clientHandler, Channel &channel, const std::string &mode,
                      const std::string &target);
    void handleTopic(ClientHandler *clientHandler, const std::string &command);
    void kickingClientFromChannel(ClientHandler *targetClient, Channel &channel);
    bool handlePasswordMode(ClientHandler *clientHandler, Channel &channel, const char modeSign,
                            const std::string &param);
};

#endif
