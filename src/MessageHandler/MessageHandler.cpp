#include "MessageHandler.hpp"

static const int RPL_WELCOME = 1;
static const int RPL_YOURHOST = 2;
static const int RPL_MYINFO = 4;
static const int RPL_NICK = 5;
static const int RPL_NICKCHANGE = 6;
static const int RPL_WELCOMECHANNEL = 7;
static const int RPL_LISTSTART = 321;

void MessageHandler::sendMessage(int socket, const std::string &message) {
    ssize_t bytesSent = send(socket, message.c_str(), message.size(), 0);
    if (bytesSent == -1) {
        std::cerr << "Error: Failed to send message to client. errno: " << errno << std::endl;
    } else {
        std::cout << "Message sent: " << message << " (" << bytesSent << " bytes)" << std::endl;
    }
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

void MessageHandler::sendUserMsg(ClientHandler *target, const std::string &message, const std::string &sender) {
    const std::string targetnickname = target->getUser().getNickname();
    const std::string response = ":" + sender + " PRIVMSG " + targetnickname + " :" + message + "\r\n";
    sendMessage(target->getSocket(), response);
}

void MessageHandler::sendResponse(ClientHandler *clientHandler, const std::string &message)
{
    sendResponse(clientHandler, 0, message);
}