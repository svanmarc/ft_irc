#include "CommandHandler.hpp"

#include "CommandHandler.hpp"

void CommandHandler::channelModelHandler(ClientHandler *clientHandler, Channel &channel, const std::string &mode,
                                         const std::string &param) {
    try {
        if (mode.size() < 2) {
            throw std::invalid_argument("Invalid mode format");
        }

        char modeChar = mode[1];
        char modeSign = mode[0];

        // Gestion des différents modes avec signe (+ ou -)
        if (modeChar == 'i') {
            channel.setInviteOnly(modeSign == '+');
        } else if (modeChar == 't') {
            channel.setTopicProtection(modeSign == '+');
        } else if (modeChar == 'k') {
            if (modeSign == '+') {
                if (!param.empty()) {
                    channel.setPassword(param);
                    std::cout << "Password set for channel: " << param << std::endl;
                } else {
                    throw std::invalid_argument("Password required for +k mode");
                }
            } else {
                channel.setPassword("");
                std::cout << "Password removed for channel" << std::endl;
            }
        } else if (modeChar == 'o') {
            channel.setOperatorPrivileges(modeSign == '+');
        } else if (modeChar == 'l') {
            if (modeSign == '+') {
                if (!param.empty()) {
                    int limit = std::atoi(param.c_str());
                    channel.setUserLimit(limit);
                    std::cout << "User limit set to: " << limit << std::endl;
                } else {
                    throw std::invalid_argument("User limit required for +l mode");
                }
            } else {
                channel.removeUserLimit();
                std::cout << "User limit removed for channel" << std::endl;
            }
        } else {
            throw std::invalid_argument("Unknown mode");
        }

        // Diffuser le changement de mode à tous les utilisateurs du canal
        if (clientHandler) {
            MessageHandler::sendChannelModes(clientHandler, channel);
        } else {
            std::cerr << "ClientHandler is null, cannot send mode update message." << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw std::invalid_argument("Failed to process mode for channel");
    }
}


void CommandHandler::userModeHandler(ClientHandler *clientHandler, const std::string &mode) {
    try {
        clientHandler->getUser().setUserMode(mode);
    } catch (const std::exception &e) {
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
    std::string param = parts.size() > 3 ? parts[3] : "";

    try {
        if (target[0] == '#') {
            Channel &channel = clientHandler->getServer()->getChannel(target);
            if (!channel.checkIfClientIsInChannel(clientHandler)) {
                MessageHandler::sendErrorNotInChannel(clientHandler, target);
                return;
            }
            channelModelHandler(clientHandler, channel, mode, param);
        } else {
            if (target != clientHandler->getUser().getNickname()) {
                std::cerr << "Can't change mode for other users" << std::endl;
                MessageHandler::sendErrorNoChangeModeForOther(clientHandler);
                return;
            }
            userModeHandler(clientHandler, mode);
        }
        MessageHandler::sendModeChange(clientHandler, mode, target);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        MessageHandler::sendErrorNoSuchChannel(clientHandler, target);
    }
}
