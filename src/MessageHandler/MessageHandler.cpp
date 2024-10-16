#include "MessageHandler.hpp"

static const int RPL_WELCOME = 1;
static const int RPL_YOURHOST = 2;
static const int RPL_MYINFO = 4;
static const int RPL_NICK = 5;
static const int RPL_NICKCHANGE = 6;
static const int RPL_WELCOMECHANNEL = 7;
static const int RPL_LISTSTART = 321;

void MessageHandler::sendMessage(int socket, const std::string &message) {
    std::cout << "Sending message to client: " << message << std::endl;

    size_t totalSent = 0;
    const char* buffer = message.c_str();
    size_t length = message.size();

    while (totalSent < length) {
        ssize_t bytesSent = send(socket, buffer + totalSent, length - totalSent, 0);

        if (bytesSent == -1) {
            if (errno == EINTR) {
                // L'appel a été interrompu par un signal, on réessaie
                continue;
            }
            std::cerr << "Error: Failed to send message to client. errnum: " << errno
                      << " (" << strerror(errno) << ")" << std::endl;
            return;
        } else if (bytesSent == 0) {
            std::cerr << "Connection closed by peer" << std::endl;
            return;
        }

        totalSent += bytesSent;
    }

    std::cout << "Message sent successfully: " << message << " (" << totalSent << " bytes)" << std::endl;
    return;
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
    std::cout << "Sending response: " << response <<  "Socket "<< clientHandler->getSocket() << std::endl;
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