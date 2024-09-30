#pragma once
#define COMMANDHANDLER_HPP
#include "ClientHandler.hpp"
#include <string>

class CommandHandler
{
public:
    static void handleCommand(const std::string& command, int clientSocket, ClientHandler *clientHandler);
};
