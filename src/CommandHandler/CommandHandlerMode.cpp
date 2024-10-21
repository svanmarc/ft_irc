#include "CommandHandler.hpp"

void CommandHandler::channelModelHandler(Channel &channel, std::string mode) {
    try {

        if (mode == "+i") {
            if (channel.getInviteOnly())
                return;
            channel.setInviteOnly(true);
        }
        else if (mode == "-i") {
            if (!channel.getInviteOnly())
                return;
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
    }
    catch (const std::exception &e) {
        throw std::invalid_argument("User not found");
    }
}

void CommandHandler::handleMode(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 3 || parts[1].empty() || parts[2].empty()) {
        MessageHandler::sendErrorModeParams(clientHandler);
        return;
    }

    std::string mode = parts[2];
    std::string target = parts[1];
    try {
        if (parts[1][0] == '#') {
            std::string channelName = parts[1];
            Channel &channel = clientHandler->getServer()->getChannel(channelName);
            if (!channel.checkIfClientIsInChannel(clientHandler)) {
                MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
                return;
            }
            channelModelHandler(channel, mode);
        }
        else {
            if (target != clientHandler->getUser().getNickname()) {
                std::cerr << "Can't change mode for other users" << std::endl;
                MessageHandler::sendErrorNoChangeModeForOther(clientHandler);
                return;
            }
            userModeHandler(clientHandler, mode);
        }
        MessageHandler::sendModeChange(clientHandler, mode, target);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        MessageHandler::sendErrorNoSuchChannel(clientHandler, target);
    }
}