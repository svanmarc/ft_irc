#include "CommandHandler.hpp"

void CommandHandler::handleMode(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 3) {
        std::string errorMsg = "Mode :Not enough parameters";
        MessageHandler::sendMessageToClient(clientHandler, errorMsg);
        MessageHandler::sendErrorModeNeedMoreParams(clientHandler);
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
        std::string errorMsg = "Mode " + channel.getName() + " :You're not a channel operator";
        MessageHandler::sendMessageToClient(clientHandler, errorMsg);
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
        std::string errorMsg = "MODE " + channel.getName() + " " + sign + "o :Need to give the nickname of the user";
        MessageHandler::sendMessageToClient(clientHandler, errorMsg);
        MessageHandler::sendErrorNotEnoughParams(clientHandler);
        return;
    }
    if (ClientHandler *targetClient = clientHandler->getServer()->findClientByNickname(param)) {
        if (!channel.checkIfClientIsInChannel(targetClient)) {
            std::string errorMsg = "MODE " + channel.getName() + " " + sign + "o " + param + " :User not in channel";
            MessageHandler::sendMessageToClient(clientHandler, errorMsg);
            MessageHandler::sendErrorNotInChannel(clientHandler, channel.getName());
            return;
        }
        if (modeSign == '+') {
            channel.addOperator(targetClient);
            MessageHandler::sendOpMode(clientHandler, targetClient, channel, sign);
        } else {
            channel.removeOperator(targetClient);
            MessageHandler::sendOpMode(clientHandler, targetClient, channel, sign);
        }
    } else {
        std::string errorMsg = "MODE " + channel.getName() + " " + sign + "o " + param + " :No such nick/channel";
        MessageHandler::sendMessageToClient(clientHandler, errorMsg);
        MessageHandler::sendErrorNoSuchNick(clientHandler);
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

    std::string pwError = "MODE " + channel.getName() + " +k " + param;

    if (modeSign == '+') {
        if (param.empty()) {
            pwError += ":Password cannot be empty";
            MessageHandler::sendMessageToClient(clientHandler, pwError);
            return false;
        }
        if (param.find(' ') != std::string::npos) {
            pwError += ":Password cannot contain spaces";
            MessageHandler::sendMessageToClient(clientHandler, pwError);
            return false;
        }
        for (std::string::size_type i = 0; i < param.size(); ++i) {
            char c = param[i];
            if (!isalnum(c)) {
                pwError += ":Password must contain only alphanumeric characters";
                MessageHandler::sendMessageToClient(clientHandler, pwError);
                return false;
            }
        }
        if (param.size() < 3 || param.size() > 20) {
            pwError += ":Password must be between 3 and 20 characters";
            MessageHandler::sendMessageToClient(clientHandler, pwError);
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
    std::string limitError = "MODE " + channel.getName() + " +l " + param;

    if (modeSign == '+') {
        if (param.empty()) {
            limitError += ":Limit cannot be empty";
            MessageHandler::sendMessageToClient(clientHandler, limitError);
            return false;
        }
        if (param.find(' ') != std::string::npos) {
            limitError += ":Limit cannot contain spaces";
            MessageHandler::sendMessageToClient(clientHandler, limitError);
            return false;
        }
        for (std::string::size_type i = 0; i < param.size(); ++i) {
            char c = param[i];
            if (!isdigit(c)) {
                limitError += ":Limit must contain only numeric characters";
                MessageHandler::sendMessageToClient(clientHandler, limitError);
                return false;
            }
        }
        int limit = std::atoi(param.c_str());
        if (limit < 1 || limit > 15) {
            limitError += ":Limit must be between 1 and 15";
            MessageHandler::sendMessageToClient(clientHandler, limitError);
            return false;
        }
        channel.setUserLimit(limit);
        return true;

    } else {
        channel.removeUserLimit();
        return true;
    }
}
