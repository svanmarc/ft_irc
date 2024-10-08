#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "ClientHandler.hpp"
#include "Server.hpp"
#include "Utils.hpp"

class Server;

class CommandHandler {
public:
    CommandHandler(Server &server);
    void handleCommand(const std::string &command, int clientSocket, ClientHandler *clientHandler);

private:
    Server &m_server;
    static const std::string WELCOME_MESSAGE;
    static const int RPL_WELCOME = 1;
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

    void sendResponse(int clientSocket, int code, const std::string &message);
    void sendResponse(int clientSocket, const std::string &message);
    std::string parseCommand(const std::string &fullCommand);
    void handleCap(const std::string &command, int clientSocket);
    void handleNick(const std::string &command, int clientSocket, ClientHandler *clientHandler);
    void handleMode(const std::string &command, int clientSocket, ClientHandler *clientHandler);
    void handleWhois(const std::string &command, int clientSocket, ClientHandler *clientHandler);
    void handleUser(const std::string &command, int clientSocket, ClientHandler *clientHandler);
    void completeRegistration(int clientSocket, ClientHandler *clientHandler);
    void handlePass(const std::string &command, int clientSocket, ClientHandler *clientHandler);
    void handleQuit(int clientSocket, ClientHandler *clientHandler);
    void handlePrivMsg(const std::string &command, int clientSocket, ClientHandler *clientHandler);
};

#endif
