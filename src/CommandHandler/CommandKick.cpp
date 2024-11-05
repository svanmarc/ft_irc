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
    std::string reason = (parts.size() > 3) && (!parts[3].empty()) ? parts[3] : "No reason given";
    Channel &channel = clientHandler->getServer()->getChannel(channelName);
    if (!channel.checkIfClientIsOperator(clientHandler)) {
        MessageHandler::sendErrorNotChannelOperator(clientHandler);
        return;
    }
    ClientHandler *targetClient = NULL;
    if (clientHandler->getServer()->getUserByNickname(targetNickname, targetClient)) {
        if (targetClient == channel.getOwner()) {
            MessageHandler::sendErrorKickOwner(clientHandler);
            return;
        }
        if (!channel.checkIfClientIsInChannel(targetClient)) {
            MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
            return;
        }
        // Message pour tous les clients (y compris le client kické)
        std::string kickMessage = "KICK " + channelName + " " + targetNickname + " :" + reason;
        kickMessage = MessageHandler::messageWithServerPrefixAndSender(clientHandler, kickMessage);
        MessageHandler::sendMessageToAllClientsInChannel(channel, kickMessage, clientHandler, targetClient);
        // Retirez le client du canal
        targetClient->leaveChannel(channelName);
        channel.removeClient(targetClient);
        channel.removeInvitedClient(targetClient);
        targetClient->setInvited(false);
        std::cout << BLUE << "💣 Client " << MAGENTA << targetClient->getNickname() << BLUE
                  << " a été expulsé du canal " << channelName << RESET << std::endl;
    } else {
        MessageHandler::sendErrorNoSuchNick(clientHandler, targetNickname);
    }
}
