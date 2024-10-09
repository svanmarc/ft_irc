#include "MessageHandler.hpp"

static const int RPL_WELCOME = 1;
static const int RPL_YOURHOST = 2;
static const int RPL_MYINFO = 4;
static const int RPL_NICK = 5;
static const int RPL_NICKCHANGE = 6;
static const int RPL_WELCOMECHANNEL = 7;
static const int RPL_LISTSTART = 321;

void MessageHandler::sendMessage(int socket, const std::string &message) {
    send(socket, message.c_str(), message.size(), 0);
}

void MessageHandler::sendResponse(ClientHandler *clientHandler, int code, const std::string &message) {
    const std::string SERVER_NAME = clientHandler->getServer()->getServerName();
    std::string response = ":" + SERVER_NAME + " ";
    if (code != 0) // N'ajouter le code que s'il est différent de 0
    {
        std::ostringstream oss;
        oss << code;
        response += oss.str() + " ";
    }
    response += message + "\r\n";
    std::cout << "Sending response: " << response << std::endl;
    MessageHandler::sendMessage(clientHandler->getSocket(), response);
}

void MessageHandler::sendResponse(ClientHandler *clientHandler, const std::string &message)
{
    sendResponse(clientHandler, 0, message);
}