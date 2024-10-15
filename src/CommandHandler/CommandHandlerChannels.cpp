#include "CommandHandler.hpp"

void CommandHandler::handleJoinChannel(const std::string &command, ClientHandler *clientHandler) {
    std::string channelName = command.substr(5);
    if (channelName[0] != '#') {
        MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
        return;
    }
    const bool joinStatus = clientHandler->joinChannel(channelName);
    if (joinStatus) {
        MessageHandler::sendWelcomeToChannel(clientHandler, channelName);
    } else {
        MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
    }

}