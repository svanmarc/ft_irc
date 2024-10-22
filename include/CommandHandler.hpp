#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "ClientHandler.hpp"
#include "IRCConstants.hpp"
#include "Irc.hpp"
#include "MessageHandler.hpp"
#include "Server.hpp"

class Server;

class CommandHandler {
public:
    // fichier CommandHandler.cpp
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


    // fichier CommandWhois.cpp
    void handleUser(ClientHandler *clientHandler, const std::string &command);
    void handlePing(ClientHandler *clientHandler, const std::string &command);
    void handlePass(ClientHandler *clientHandler, const std::string &command);
    void completeRegistration(ClientHandler *clientHandler);

    // fichier CommandHandler.cpp
    Server &getServer() const;

    // fichier CommandsUsers.cpp
    void handleWhois(ClientHandler *clientHandler, const std::string &command);
    std::string parseCommand(const std::string &fullCommand);

    // fichier CommandHandlerNick.cpp
    bool checkNickname(const std::string &nickname, ClientHandler *clientHandler);
    void handleNick(ClientHandler *clientHandler, const std::string &command);

    // fichier CommandHandlerMsg
    void handlePrivMsg(ClientHandler *clientHandler, const std::string &command);

    // fichier CommandHandlerChannels.cpp
    void handleJoinChannel(ClientHandler *clientHandler, const std::string &command);
    void handlePart(ClientHandler *clientHandler, const std::string &command);

    // fichier CommandHandlerMode.cpp
    void handleMode(ClientHandler *clientHandler, const std::string &command);
    void channelModelHandler(Channel &channel, std::string mode);
    void userModeHandler(ClientHandler *clientHandler, std::string mode);

    // fichier CommandHandlerQuit.cpp
    void handleQuit(ClientHandler *clientHandler, const std::string &command);
    void handleCap(ClientHandler *clientHandler, const std::string &command);

    // fichier CommandInvite.cpp
    void handleInvite(ClientHandler *clientHandler, const std::string &command);
};

#endif
