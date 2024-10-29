#include "CommandHandler.hpp"

void CommandHandler::channelModelHandler(ClientHandler *clientHandler, Channel &channel, const std::string &mode,
                                         const std::string &param) {
    try {
        if (mode.size() < 2 || (mode[0] != '+' && mode[0] != '-')) {
            throw std::invalid_argument("Invalid mode format");
        }

        const char modeChar = mode[1];
        const char modeSign = mode[0];
        std::string sign(1, modeSign);
        std::string modeStr(1, modeChar);

        if (modeChar == 'i') {
            channel.setInviteOnly(modeSign == '+');
        } else if (modeChar == 't') {
            channel.setTopicProtection(modeSign == '+');
        } else if (modeChar == 'k') {
            // if (modeSign == '+' && param.empty()) {
            //     MessageHandler::sendErrorNotEnoughParams(clientHandler);
            //     return;
            // }
            if (!handlePasswordMode(clientHandler, channel, modeSign, param)) {
                return;
            }
        } else if (modeChar == 'l') {
            if (modeSign == '+') {
                if (!param.empty()) {
                    int limit = std::atoi(param.c_str());
                    channel.setUserLimit(limit);
                } else {
                    MessageHandler::sendErrorNotEnoughParams(clientHandler);
                    return;
                }
            } else {
                channel.removeUserLimit();
            }
        } else {
            MessageHandler::sendErrorModeParams(clientHandler);
            return;
        }
        MessageHandler::sendChannelModes(clientHandler, channel, sign, modeStr);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
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
    // if (parts.size() > 4) {
    //     MessageHandler::sendErrorModeParams(clientHandler);
    //     return;
    // }

    if (mode.size() != 2) {
        MessageHandler::sendErrorBadMode(clientHandler, mode);
        return;
    }

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

bool CommandHandler::handlePasswordMode(ClientHandler *clientHandler, Channel &channel, const char modeSign,
                                        const std::string &param) {

    std::string pwError = "MODE " + channel.getName() + " +k " + param;

    if (modeSign == '+') {
        if (param.empty()) {
            pwError += ":Password cannot be empty";
            pwError = MessageHandler::messageWithServerPrefixAndSender(clientHandler, pwError);
            MessageHandler::sendMessageToClient(clientHandler, pwError);
            return false;
        }

        if (param.find(' ') != std::string::npos) {
            pwError += ":Password cannot contain spaces";
            pwError = MessageHandler::messageWithServerPrefixAndSender(clientHandler, pwError);
            MessageHandler::sendMessageToClient(clientHandler, pwError);
            return false;
        }
        for (std::string::size_type i = 0; i < param.size(); ++i) {
            char c = param[i];
            if (!isalnum(c)) {
                pwError += ":Password must contain only alphanumeric characters";
                pwError = MessageHandler::messageWithServerPrefixAndSender(clientHandler, pwError);
                MessageHandler::sendMessageToClient(clientHandler, pwError);
                return false;
            }
        }
        if (param.size() < 3 || param.size() > 20) {
            pwError += ":Password must be between 3 and 20 characters";
            pwError = MessageHandler::messageWithServerPrefixAndSender(clientHandler, pwError);
            MessageHandler::sendMessageToClient(clientHandler, pwError);
            return false;
        }
        channel.setPassword(param);
        std::cout << "Password set for channel: " << param << std::endl;
        std::string modeMessage = "MODE " + channel.getName() + " +k";
        modeMessage = MessageHandler::messageWithServerPrefixAndSender(clientHandler, modeMessage);
        MessageHandler::sendMessageToAllClientsInChannel(channel, modeMessage, clientHandler, false);
        return true;

    } else {
        channel.setPassword("");
        std::cout << "Password removed for channel" << std::endl;
        std::string modeMessage = "MODE " + channel.getName() + " -k";
        modeMessage = MessageHandler::messageWithServerPrefixAndSender(clientHandler, modeMessage);
        MessageHandler::sendMessageToAllClientsInChannel(channel, modeMessage, clientHandler, false);
        return true;
    }
}
