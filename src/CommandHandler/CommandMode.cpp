#include "CommandHandler.hpp"

void CommandHandler::channelModelHandler(ClientHandler *clientHandler, Channel &channel, const std::string &mode,
                                         const std::string &param) {
    try {
        if (mode.size() < 2) {
            throw std::invalid_argument("Invalid mode format");
        }

        char modeChar = mode[1];
        char modeSign = mode[0];
        std::string nickname = clientHandler->getUser().getNickname();
        std::string sign(1, modeSign);

        // Effectuer les modifications de mode
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

        MessageHandler::sendChannelModes(clientHandler, channel, nickname, sign);

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
        std::cout << "Handling mode for target: " << target << " with mode: " << mode << std::endl;

        if (target[0] == '#') {
            Channel &channel = clientHandler->getServer()->getChannel(target);
            if (!channel.checkIfClientIsInChannel(clientHandler)) {
                MessageHandler::sendErrorNotInChannel(clientHandler, target);
                return;
            }

            // Vérifiez si le client est opérateur pour tous les modes
            if (mode[1] == 'o' || mode[1] == 'i' || mode[1] == 't' || mode[1] == 'k' || mode[1] == 'l') {
                if (!channel.checkIfClientIsOperator(clientHandler)) {
                    std::cerr << clientHandler->getUser().getNickname() << " is not an operator, denying request."
                              << std::endl;
                    MessageHandler::sendErrorNotChannelOperator(clientHandler);
                    return;
                }
            }

            // Traitez les modes
            if (mode[1] == 'o') {
                std::string target = parts[3];
                std::cout << "Client " << clientHandler->getNickname() << " requested to change mode " << mode
                          << " for target " << target << " in channel " << channel.getName() << std::endl;
                handleOpMode(clientHandler, channel, mode, target);
            } else {
                channelModelHandler(clientHandler, channel, mode, param);
            }
        } else {
            // Gérer les modes pour les utilisateurs
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


void CommandHandler::handleOpMode(ClientHandler *clientHandler, Channel &channel, const std::string &mode,
                                  const std::string &target) {
    std::cout << "Handling op mode for target: " << target << " with mode: " << mode << std::endl;
    if (mode[0] == '+') {
        if (channel.checkIfClientIsOperator(clientHandler)) {
            ClientHandler *targetClient = clientHandler->getServer()->findClientByNickname(target);
            if (targetClient) {
                channel.addOperator(targetClient);
                MessageHandler::sendOpMode(clientHandler, targetClient, channel);
                std::cout << "Client " << targetClient->getNickname() << " is now an operator of channel "
                          << channel.getName() << std::endl;
            } else {
                MessageHandler::sendErrorNoSuchNick(clientHandler, target);
                std::cerr << "Client " << target << " not found" << std::endl;
            }
        } else {
            MessageHandler::sendErrorNotChannelOperator(clientHandler);
            std::cerr << "Client " << clientHandler->getNickname() << " is not an operator of channel "
                      << channel.getName() << std::endl;
        }
    } else {
        if (channel.checkIfClientIsOperator(clientHandler)) {
            ClientHandler *targetClient = clientHandler->getServer()->findClientByNickname(target);
            if (targetClient) {
                channel.removeOperator(targetClient);
                MessageHandler::sendOpMode(clientHandler, targetClient, channel);
                std::cout << "Client " << targetClient->getNickname() << " is no longer an operator of channel "
                          << channel.getName() << std::endl;
            } else {
                MessageHandler::sendErrorNoSuchNick(clientHandler, target);
                std::cerr << "Client " << target << " not found" << std::endl;
            }
        } else {
            MessageHandler::sendErrorNotChannelOperator(clientHandler);
            std::cerr << "Client " << clientHandler->getNickname() << " is not an operator of channel "
                      << channel.getName() << std::endl;
        }
    }
}
