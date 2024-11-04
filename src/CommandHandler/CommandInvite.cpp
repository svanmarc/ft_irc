#include "CommandHandler.hpp"

void CommandHandler::handleInvite(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 2) {
        MessageHandler::sendErrorNoTarget(clientHandler);
        return;
    }

    std::string target = trim(parts[1]);
    std::string param = (parts.size() > 2) ? trim(parts[2]) : "";

    std::string channelName;
    if (!param.empty()) {
        if (param[0] != '#' || !clientHandler->getServer()->checkIfChannelExists(param)) {
            MessageHandler::sendErrorNoSuchChannel(clientHandler, param);
            return;
        }
        channelName = param;
    } else {
        if (clientHandler->getChannels().empty()) {
            MessageHandler::sendErrorNotInChannel(clientHandler, "current");
            return;
        }
        channelName = clientHandler->getChannels().back();
    }

    Channel &channel = clientHandler->getServer()->getChannel(channelName);
    if (!channel.checkIfClientIsInChannel(clientHandler)) {
        MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
        return;
    }

    if (!channel.checkIfClientIsOperator(clientHandler)) {
        MessageHandler::sendErrorNotChannelOperator(clientHandler);
        return;
    }

    ClientHandler *targetClient = NULL;
    if (clientHandler->getServer()->getUserByNickname(target, targetClient)) {
        if (channel.checkIfClientIsInChannel(targetClient)) {
            MessageHandler::sendErrorAlreadyInChannel(clientHandler, target, channelName);
            return;
        }

        if (channel.isClientInvited(targetClient)) {
            MessageHandler::sendErrorAlreadyInvited(clientHandler, target, channelName);
            return;
        }

        channel.inviteClient(targetClient);
        MessageHandler::sendInviteNotification(clientHandler, targetClient, channel);
        std::string message = ":" + clientHandler->getNickname() + " INVITE " + target + " " + channelName + "\r\n";
        MessageHandler::sendMessageToAllClientsInChannel(channel, message, clientHandler, targetClient);
    } else {
        std::string errormsg = "INVITE " + target;
        errormsg += " :No such nick/channel";
        MessageHandler::sendMessageToClient(clientHandler, errormsg);
    }
}
