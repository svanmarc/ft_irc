#include "CommandHandler.hpp"

void CommandHandler::handleName(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() != 2) {
        MessageHandler::sendErrorModeParams(clientHandler);
        return;
    }

    std::string channelName = trim(parts[1]);

    if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
        MessageHandler::sendErrorNoSuchChannel(clientHandler, channelName);
        return;
    }

    Channel &channel = clientHandler->getServer()->getChannel(channelName);
    if (!channel.checkIfClientIsInChannel(clientHandler)) {
        MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
        return;
    }
    MessageHandler::sendCurrentMemberListToNew(clientHandler, channel);
    MessageHandler::sendEndOfNamesList(clientHandler, channel);
}

void CommandHandler::handleList(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() == 1) {
        MessageHandler::sendChannelsList(clientHandler);
        return;
    }
    if (parts.size() != 2) {
        MessageHandler::sendErrorModeParams(clientHandler);
        return;
    }

    std::string channelName = trim(parts[1]);
    if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
        MessageHandler::sendErrorNoSuchChannel(clientHandler, channelName);
        return;
    }

    Channel &channel = clientHandler->getServer()->getChannel(channelName);
    MessageHandler::sendResponse(clientHandler, 322,clientHandler->getNickname() + ": " + IRCConstants::COLOR_GREEN +  "Channel ::"  + channelName + IRCConstants::COLOR_RESET) ;
    MessageHandler::sendChannelList(clientHandler, channel);
    MessageHandler::sendEndOfList(clientHandler);
}