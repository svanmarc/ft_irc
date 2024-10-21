#include "CommandHandler.hpp"

void CommandHandler::channelModelHandler(Channel &channel, std::string mode) {
    try {
        if (mode == "+i") {
            channel.setInviteOnly(true);
        }
        else if (mode == "-i") {
            channel.setInviteOnly(false);
        }
        else {
            throw std::invalid_argument("Unknown mode");
        }
    }
    catch (const std::exception &e) {
        throw std::invalid_argument("Channel not found");
    }
}

void CommandHandler::userModeHandler(ClientHandler *clientHandler, std::string mode) {
    try {
        clientHandler->getUser().setUserMode(mode);

        std::string response = clientHandler->getNickname();
        response += " " + mode;
        MessageHandler::sendResponse(clientHandler, RPL_CHANNELMODEIS, response);
    }
    catch (const std::exception &e) {
        throw std::invalid_argument("User not found");
    }
}

void CommandHandler::handleMode(const std::string &command, ClientHandler *clientHandler) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 3 || parts[1].empty() || parts[2].empty()) {
        MessageHandler::sendErrorModeParams(clientHandler);
        return;
    }

    std::string mode = parts[2];
    std::string target = parts[1];
    if (parts[1][0] == '#') {
        std::string channelName = parts[1];
        Channel &channel = clientHandler->getServer()->getChannel(channelName);
        channelModelHandler(channel, mode);
    }
    else {
        userModeHandler(clientHandler, mode);
    }
    MessageHandler::sendModeChange(clientHandler, mode, target);

}