#include "MessageHandler.hpp"

static const int RPL_YOURHOST = 2;
static const int RPL_MYINFO = 4;
static const int RPL_NICK = 5;
static const int RPL_NICKCHANGE = 6;
static const int RPL_WELCOMECHANNEL = 7;
static const int RPL_LISTSTART = 321;

void MessageHandler::sendMessage(int socket, const std::string &message) {
    std::string finalMessage = message;

    // Ajouter \r\n si le message ne se termine pas déjà par \r\n
    if (finalMessage.substr(finalMessage.size() - 2) != "\r\n") {
        finalMessage += "\r\n";
    }

    size_t totalSent = 0;
    const char *buffer = finalMessage.c_str();
    size_t length = finalMessage.size();

    while (totalSent < length) {
        ssize_t bytesSent = send(socket, buffer + totalSent, length - totalSent, 0);

        if (bytesSent == -1) {
            if (errno == EINTR) {
                // L'appel a été interrompu par un signal, on réessaie
                continue;
            }
            std::cerr << "Error: Failed to send message to client. errnum: " << errno << " (" << strerror(errno) << ")"
                      << std::endl;
            return;
        } else if (bytesSent == 0) {
            std::cerr << "Connection closed by peer" << std::endl;
            return;
        }

        totalSent += bytesSent;
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
    response += message;
    MessageHandler::sendMessage(clientHandler->getSocket(), response);
}

void MessageHandler::sendUserMsg(ClientHandler *target, const std::string &message, ClientHandler *sender, bool notice) {
    //:tamm2_!root@IP.hosted-by-42lausanne.ch NOTICE tamm2 :tu es dans la merde
    const std::string targetnickname = target->getUser().getNickname();
    std::string msgType = "PRIVMSG";
    if (notice) {
        msgType = "NOTICE " + message;
    }

    std::string response = ":" + sender->getUser().getNickname();
    response += "!" + sender->getUser().getUsername() + "@" + sender->getUser().getHostname();
    response +=  " " + msgType + " " + targetnickname + " :" + message + "\r\n";
    sendMessage(target->getSocket(), response);
}

void MessageHandler::sendResponse(ClientHandler *clientHandler, const std::string &message) {
    sendResponse(clientHandler, 0, message);
}