#include "MessageHandler.hpp"

std::string MessageHandler::messageWithServerPrefixAndSender(ClientHandler *clientHandler, std::string &originalMessage) {
    std::string message = ":" + clientHandler->getUser().getNickname() + "!";
    message += clientHandler->getUser().getUsername() + "@";
    message += clientHandler->getUser().getHostname();
    message += " " + originalMessage;
    return message;
}