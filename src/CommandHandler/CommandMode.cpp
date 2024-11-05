#include "CommandHandler.hpp"
#include "IRCConstants.hpp"

void CommandHandler::handleMode(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 3) {
        return;
    }
    std::string target = parts[1];
    std::string mode = parts[2];
    std::string param = parts.size() > 3 ? parts[3] : "";

    // Vérifier si la cible est un canal
    if (target[0] == '#') {
        if (!verifyClientInChannel(clientHandler, target))
            return;

        Channel &channel = clientHandler->getServer()->getChannel(target);

        if (!verifyClientIsOperator(clientHandler, channel))
            return;
        if (!validateModeFormat(mode)) {
            std::string errorMsg = "Mode " + mode + " :Unknown mode";
            MessageHandler::sendMessageToClient(clientHandler, errorMsg);
            MessageHandler::sendErrorBadMode(clientHandler, mode);
            return;
        }
        applyModeToChannel(clientHandler, channel, mode, param);
    } else {
        handleUserMode(clientHandler, target, mode);
    }
}

bool CommandHandler::verifyClientInChannel(ClientHandler *clientHandler, std::string &channelName) {
    try {
        Channel &channel = clientHandler->getServer()->getChannel(channelName);
        if (!channel.checkIfClientIsInChannel(clientHandler)) {
            std::string errorMsg = "Mode " + channelName + " :You're not in that channel";
            MessageHandler::sendMessageToClient(clientHandler, errorMsg);
            MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
            return false;
        }
        return true;
    } catch (const std::exception &) {
        MessageHandler::sendErrorNoSuchChannel(clientHandler, channelName);
        return false;
    }
}

bool CommandHandler::verifyClientIsOperator(ClientHandler *clientHandler, Channel &channel) {
    if (!channel.checkIfClientIsOperator(clientHandler)) {
        MessageHandler::sendErrorNotChannelOperator(clientHandler);
        return false;
    }
    return true;
}

bool CommandHandler::validateModeFormat(const std::string &mode) {
    return (mode.size() == 2 && (mode[0] == '+' || mode[0] == '-') && isValidModeChar(mode[1]));
}

void CommandHandler::applyModeToChannel(ClientHandler *clientHandler, Channel &channel, const std::string &mode,
                                        const std::string &param) {
    if (mode[1] == 'o') {
        handleOpMode(clientHandler, channel, mode, param);
    } else {
        channelModelHandler(clientHandler, channel, mode, param);
    }
}

void CommandHandler::handleUserMode(ClientHandler *clientHandler, const std::string &target, const std::string &mode) {
    if (target != clientHandler->getUser().getNickname()) {
        MessageHandler::sendErrorNoChangeModeForOther(clientHandler);
        return;
    }
    userModeHandler(clientHandler, mode);
}

void CommandHandler::channelModelHandler(ClientHandler *clientHandler, Channel &channel, const std::string &mode,
                                         const std::string &param) {
    try {
        char modeChar = mode[1];
        char modeSign = mode[0];

        if (modeChar == 'i') {
            channel.setInviteOnly(modeSign == '+');
        } else if (modeChar == 't') {
            channel.setTopicProtection(modeSign == '+');
        } else if (modeChar == 'k') {
            if (!handlePasswordMode(clientHandler, channel, modeSign, param)) {
                return;
            }
        } else if (modeChar == 'l') {
            if (!handleLimitMode(clientHandler, channel, modeSign, param)) {
                return;
            }
        }
        MessageHandler::sendChannelModes(clientHandler, channel, std::string(1, modeSign), std::string(1, modeChar));
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void CommandHandler::handleOpMode(ClientHandler *clientHandler, Channel &channel, const std::string &mode,
                                  const std::string &param) {
    const char modeSign = mode[0];
    std::string sign(1, modeSign);
    if (param.empty()) {
        MessageHandler::sendErrorModeWithMessage(clientHandler, "Need to give the nickname of the user", mode, channel, "");
        return;
    }
    if (ClientHandler *targetClient = clientHandler->getServer()->findClientByNickname(param)) {
        if (!channel.checkIfClientIsInChannel(targetClient)) {
            MessageHandler::sendErrorModeWithMessage(clientHandler, "User not in channel", mode, channel, param);
            return;
        }
        if (modeSign == '+') {
            channel.addOperator(targetClient);
            MessageHandler::sendOpMode(clientHandler, targetClient, channel, sign);
        } else {
            if (targetClient == channel.getOwner()) {
                MessageHandler::sendErrorModeWithMessage(clientHandler, "You can't take operator status from the owner",
                                                         mode, channel, param);
                return;
            }
            channel.removeOperator(targetClient);
            MessageHandler::sendOpMode(clientHandler, targetClient, channel, sign);
        }
    } else {
        MessageHandler::sendErrorModeWithMessage(clientHandler, "No such nick/channel", mode, channel, param);
    }
}

bool CommandHandler::isValidModeChar(char modeChar) {
    return modeChar == 'o' || modeChar == 'i' || modeChar == 't' || modeChar == 'k' || modeChar == 'l';
}

void CommandHandler::userModeHandler(ClientHandler *clientHandler, const std::string &mode) {
    try {
        clientHandler->getUser().setUserMode(mode);
    } catch (const std::exception &e) {
        throw std::invalid_argument("User not found");
    }
}

bool CommandHandler::handlePasswordMode(ClientHandler *clientHandler, Channel &channel, const char modeSign,
                                        const std::string &param) {
    std::ostringstream oss;
    oss << modeSign << 'k';
    std::string mode = oss.str();

    if (modeSign == '+') {
        if (param.empty()) {
            MessageHandler::sendErrorModeWithMessage(clientHandler, "Password cannot be empty", mode, channel, param);
            return false;
        }
        if (param.find(' ') != std::string::npos) {
            MessageHandler::sendErrorModeWithMessage(clientHandler, "Password cannot contain spaces", mode, channel, param);
            return false;
        }
        for (std::string::size_type i = 0; i < param.size(); ++i) {
            char c = param[i];
            if (!isalnum(c)) {
                MessageHandler::sendErrorModeWithMessage(clientHandler, "Password must contain only alphanumeric characters",
                                                         mode, channel, param);
                return false;
            }
        }
        if (param.size() < 3 || param.size() > 20) {
            MessageHandler::sendErrorModeWithMessage(clientHandler, "Password must be between 3 and 20 characters",
                                                     mode, channel, param);
            return false;
        }
        channel.setPassword(param);
        return true;

    } else {
        channel.setPassword("");
        return true;
    }
}

bool CommandHandler::handleLimitMode(ClientHandler *clientHandler, Channel &channel, const char modeSign,
                                     const std::string &param) {
    std::ostringstream oss;
    oss << modeSign << 'l';
    std::string mode = oss.str();

    if (modeSign == '+') {
        if (param.empty()) {
            MessageHandler::sendErrorModeWithMessage(clientHandler, "Limit cannot be empty",
                                                     mode, channel, param);
            return false;
        }
        if (param.find(' ') != std::string::npos) {
            MessageHandler::sendErrorModeWithMessage(clientHandler, "Limit cannot contain spaces",
                                                     mode, channel, param);
            return false;
        }
        for (std::string::size_type i = 0; i < param.size(); ++i) {
            char c = param[i];
            if (!isdigit(c)) {
                MessageHandler::sendErrorModeWithMessage(clientHandler, "Limit must contain only numeric characters",
                                                     mode, channel, param);
                return false;
            }
        }
        int limit = std::atoi(param.c_str());
        if (limit < 1 || limit > 15) {
            MessageHandler::sendErrorModeWithMessage(clientHandler, "Limit must be between 1 and 15",
                                                     mode, channel, param);
            return false;
        }
        channel.setUserLimit(limit);
        return true;

    } else {
        channel.removeUserLimit();
        return true;
    }
}
