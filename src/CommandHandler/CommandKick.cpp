#include "CommandHandler.hpp"

void CommandHandler::handleKick(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 3) {
        MessageHandler::sendErrorNoTarget(clientHandler);
        return;
    }

    std::string channelName = parts[1];
    std::string targetNickname = parts[2];

    Channel &channel = clientHandler->getServer()->getChannel(channelName);

    if (!channel.checkIfClientIsOperator(clientHandler)) {
        MessageHandler::sendErrorNotChannelOperator(clientHandler);
        return;
    }

    ClientHandler *targetClient;
    if (clientHandler->getServer()->getUserByNickname(targetNickname, targetClient)) {
        if (!channel.checkIfClientIsInChannel(targetClient)) {
            MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
            return;
        }

        // Envoyer un message de notification au client expulsé
        std::string kickMessageToTarget = ":" + clientHandler->getNickname() + "!" +
                                          clientHandler->getUser().getUsername() + "@" +
                                          clientHandler->getUser().getHostname() + " KICK " + channelName + " :" +
                                          targetNickname + " has been kicked from the channel";
        MessageHandler::sendMessageToClient(targetClient, kickMessageToTarget);

        // Envoyer un message aux autres clients dans le canal
        std::string kickMessageToOthers =
                ":" + clientHandler->getNickname() + " KICK " + channelName + " " + targetNickname;
        MessageHandler::sendMessageToAllClientsInChannel(channel, kickMessageToOthers, clientHandler, targetClient);

        // Retirez le client du canal
        targetClient->leaveChannel(channelName);
        channel.removeClient(targetClient);
        targetClient->setInvited(false);
    } else {
        MessageHandler::sendErrorNoSuchNick(clientHandler, targetNickname);
    }
}
