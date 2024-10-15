#include "CommandHandler.hpp"

void CommandHandler::handleJoinChannel(const std::string &command, ClientHandler *clientHandler) {
    std::string channelName = command.substr(5);
    if (channelName[0] != '#') {
        MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
        return;
    }
    const bool joinStatus = clientHandler->joinChannel(channelName);
    if (joinStatus) {
        Channel const newChannel = clientHandler->getServer()->getChannel(channelName);
        MessageHandler::sendWelcomeToChannel(clientHandler, newChannel);

    } else {
        MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
    }

}