#pragma once
#define COMMANDHANDLER_HPP
#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <vector>
#include "ClientHandler.hpp"
#include "Server.hpp"
#include "Utils.hpp"

class CommandHandler {
public:
    static void handleCommand(const std::string &command, int clientSocket, ClientHandler *clientHandler);

private:
    static const int RPL_WELCOME = 1;
    static const int RPL_LISTSTART = 321;
    static const int RPL_CAPLS = 375;
    static const int RPL_CHANNELMODEIS = 324;
    static const int ERR_UNKNOWNCOMMAND = 421;
    static const int RPL_WHOISUSER = 311;
    static const int ERR_NEEDMOREPARAMS = 461;
    static const int ERR_ALREADYREGISTRED = 462;
    static const std::string WELCOME_MESSAGE;

    static void sendResponse(int clientSocket, int code, const std::string &message);
    static void sendResponse(int clientSocket, const std::string &message);
    static std::string parseCommand(const std::string &fullCommand);
    static void handleCap(const std::string &command, int clientSocket);
    static void handleNick(const std::string &command, int clientSocket, ClientHandler *clientHandler);
    static void handleMode(const std::string &command, int clientSocket, ClientHandler *clientHandler);
    static void handleWhois(int clientSocket, ClientHandler *clientHandler);
    static void handleUser(const std::string &command, int clientSocket, ClientHandler *clientHandler);
    static void completeRegistration(int clientSocket, ClientHandler *clientHandler);
};
