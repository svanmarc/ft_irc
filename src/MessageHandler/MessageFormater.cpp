#include "MessageHandler.hpp"

std::string MessageHandler::messageWithServerPrefixAndSender(ClientHandler *clientHandler, std::string &originalMessage) {
    std::string message = ":" + clientHandler->getUser().getUsername() + "!";
    message += clientHandler->getUser().getNickname() + "@";
    message += clientHandler->getUser().getHostname();
    message += " " + originalMessage;
    return message;
}

std::string MessageHandler::MessageWhoisFormat(ClientHandler *clientHandler, ClientHandler *targetClient) {
    std::string response = std::string(clientHandler->getNickname() + " ");
    response += std::string(targetClient->getUser().getNickname() + " ");
    return response;
}