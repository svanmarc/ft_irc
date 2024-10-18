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
    CommandHandler(Server &server);
    void handleCommand(const std::string &command, ClientHandler *clientHandler);
    void handleCommandNoRegistred(const std::string &command, ClientHandler *clientHandler);
    void handleCommandNoAuth(const std::string &command, ClientHandler *clientHandler);
    void handleCommandNoAuthentificated(const std::string &command, ClientHandler *clientHandler);

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
    static const int ERR_ALREADYREGISTRED = 462;


    // fichier CommandWhois.cpp
    static void handleUser(const std::string &command, ClientHandler *clientHandler);
    static void handlePing(const std::string &command, ClientHandler *clientHandler);
    void handlePass(const std::string &command, ClientHandler *clientHandler);
    static void completeRegistration(ClientHandler *clientHandler);
    // fichier CommandHandler.cpp
    static void handleMode(const std::string &command, ClientHandler *clientHandler);
    void handleQuit(ClientHandler *clientHandler);
    Server &getServer() const;
    static void handleCap(ClientHandler *clientHandler, const std::string &command);
    // fichier CommandsUsers.cpp
    void handleWhois(const std::string &command, ClientHandler *clientHandler);
    static std::string parseCommand(const std::string &fullCommand);


    // fichier CommandHandlerNick.cpp
    static bool checkNickname(const std::string &nickname, ClientHandler *clientHandler);
    static void handleNick(const std::string &command, ClientHandler *clientHandler);

    // fichier CommandHandlerMsg
    void handlePrivMsg(const std::string &command, ClientHandler *clientHandler);

    // fichier CommandHandlerChannels.cpp
    void handleJoinChannel(const std::string &command, ClientHandler *clientHandler);
    void handlePart(const std::string &command, ClientHandler *clientHandler);
};

#endif
