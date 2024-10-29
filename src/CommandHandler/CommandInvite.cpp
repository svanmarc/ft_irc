#include "CommandHandler.hpp"

void CommandHandler::handleInvite(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    std::string channelName;

    if (parts.size() < 2 || parts.size() > 3) {
        if (parts.size() < 2)
            MessageHandler::sendErrorNoTarget(clientHandler);
        else if (parts.size() > 3)
            MessageHandler::sendErrortooManyTargets(clientHandler);
        return;
    }

    std::string targetNickname = trim(parts[1]);

    // pour nc
    if (parts.size() == 3) {
        if (parts[2][0] != '#') {
            MessageHandler::sendErrorNoSuchChannel(clientHandler, parts[2]);
            return;
        } else
            channelName = trim(parts[2]);
    }
    // pour irssi
    else {
        if (clientHandler->getChannels().empty()) {
            MessageHandler::sendErrorNotInChannel(clientHandler, "current");
            return;
        }
        channelName = clientHandler->getChannels().back();
    }


    if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
        MessageHandler::sendErrorNoSuchChannel(clientHandler, channelName);
        return;
    }

    Channel &channel = clientHandler->getServer()->getChannel(channelName);
    if (!channel.checkIfClientIsInChannel(clientHandler)) {
        MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
        return;
    }

    ClientHandler *targetClient = NULL;
    if (clientHandler->getServer()->getUserByNickname(targetNickname, targetClient)) {
        if (channel.getInviteOnly() && !channel.checkIfClientIsOperator(clientHandler)) {
            MessageHandler::sendErrorNotChannelOperator(clientHandler);
            return;
        }
        if (channel.checkIfClientIsInChannel(targetClient)) {
            MessageHandler::sendErrorAlreadyInChannel(clientHandler, targetNickname, channelName);
            return;
        }

        if (channel.isClientInvited(targetClient)) {
            MessageHandler::sendErrorAlreadyInvited(clientHandler, targetNickname, channelName);
            return;
        }

        channel.inviteClient(targetClient);
        MessageHandler::sendInviteNotification(clientHandler, targetClient, channel);
        std::string message =
                ":" + clientHandler->getNickname() + " INVITE " + targetNickname + " " + channelName + "\r\n";
        MessageHandler::sendMessageToAllClientsInChannel(channel, message, clientHandler, targetClient);
    } else {
        MessageHandler::sendErrorNoSuchNick(clientHandler, targetNickname);
    }
}
