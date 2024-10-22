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
            // L'utilisateur n'est dans aucun canal, envoyer une erreur
            MessageHandler::sendErrorNotInChannel(clientHandler, "current");
            return;
        }
        // Utiliser le canal actuel de l'utilisateur
        channelName = clientHandler->getChannels().back();
    }

    // Vérifier si le canal existe
    if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
        MessageHandler::sendErrorNoSuchChannel(clientHandler, channelName);
        return;
    }

    // Vérifier si l'utilisateur qui envoie l'invitation est dans le canal
    Channel &channel = clientHandler->getServer()->getChannel(channelName);
    if (!channel.checkIfClientIsInChannel(clientHandler)) {
        MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
        return;
    }

    // Récupérer l'utilisateur cible
    ClientHandler *targetClient = NULL;
    if (clientHandler->getServer()->getUserByNickname(targetNickname, targetClient)) {
        // Vérifier si l'utilisateur cible est déjà dans le canal
        if (channel.checkIfClientIsInChannel(targetClient)) {
            MessageHandler::sendErrorAlreadyInChannel(clientHandler, targetNickname, channelName);
            return;
        }

        // Vérifier si l'utilisateur est déjà invité
        if (channel.isClientInvited(targetClient)) {
            MessageHandler::sendErrorAlreadyInvited(clientHandler, targetNickname, channelName);
            return;
        }

        // Envoyer l'invitation
        channel.inviteClient(targetClient);
        MessageHandler::sendInviteNotification(clientHandler, targetClient, channel);
        std::string message =
                ":" + clientHandler->getNickname() + " INVITE " + targetNickname + " " + channelName + "\r\n";
        MessageHandler::sendMessageToAllClientsInChannel(channel, message, clientHandler, targetClient);
    } else {
        MessageHandler::sendErrorNoSuchNick(clientHandler, targetNickname);
    }
}
