#include "MessageHandler.hpp"

void MessageHandler::sendWelcomeToChannel(ClientHandler *clientHandler, const Channel &channel) {
    std::string topic = channel.getTopic() == "" ? "No topic is set" : channel.getTopic();
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    message += " " + channel.getTopic();
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WELCOME, message);
}

void MessageHandler::sendNewMemberToChannel(ClientHandler *clientHandler, const Channel &channel) {
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    for (std::vector<User>::const_iterator it = channel.getUsers().begin(); it != channel.getUsers().end(); ++it) {
        sendUserMsg((*it),message, clientHandler->getUser().getNickname());
    }
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_JOIN, message);
}