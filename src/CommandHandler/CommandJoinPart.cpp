#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

void CommandHandler::handleJoinChannel(ClientHandler *clientHandler, const std::string &command) {
    std::cout << "Handling JOIN command: " << command << " from " << clientHandler->getNickname() << std::endl;
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 2) {
        MessageHandler::sendErrorJoinChannel(clientHandler, "");
        return;
    }

    std::string channelName = trim(parts[1]);
    if (channelName[0] != '#') {
        MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
        return;
    }

    // Vérification si le canal existe et gestion des permissions
    try {
        Channel &channel = clientHandler->getServer()->getChannel(channelName);
        if (channel.getInviteOnly()) {
            if (!channel.isClientInvited(clientHandler)) {
                MessageHandler::sendErrorInviteOnly(clientHandler, channelName);
                return;
            }
        }
    } catch (const std::runtime_error &e) {
        // Le canal n'existe pas encore, nous allons le créer
        std::cout << "Channel not found. Creating new channel." << std::endl;
    }

    // Validation de l'état de la connexion au canal
    if (!clientHandler->getServer()->joinChannel(clientHandler, channelName)) {
        return;
    }

    // Une fois la connexion validée, ajouter le client à la liste des canaux
    Channel &newChannel = clientHandler->getServer()->getChannel(channelName);
    clientHandler->addChannelToList(channelName);

    // Envoi des messages aux autres membres du canal
    MessageHandler::sendWelcomeToChannel(clientHandler, newChannel);
    MessageHandler::sendNewMemberToChannel(clientHandler, newChannel);
    MessageHandler::sendCurrentMemberListToNew(clientHandler, newChannel);
    MessageHandler::sendEndOfNamesList(clientHandler, newChannel);
}


void CommandHandler::handlePart(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 2) {
        MessageHandler::sendErrorNoTarget(clientHandler);
        return;
    }

    std::string channelName = trim(parts[1]);

    // Extraire le message après le symbole ':', s'il existe
    std::string partMessage = "Left the channel";
    if (parts.size() > 2) {
        partMessage = command.substr(command.find(':') + 1);
    }

    if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
        MessageHandler::sendErrorNoSuchNick(clientHandler, channelName);
        return;
    }

    Channel &channel = clientHandler->getServer()->getChannel(channelName);
    std::string leaveMessage = ":" + clientHandler->getNickname() + "!" + clientHandler->getUser().getUsername() + "@" +
                               clientHandler->getUser().getHostname() + " PART " + channelName + " :" + partMessage;

    MessageHandler::sendMessageToAllClientsInChannel(channel, leaveMessage, clientHandler, true);

    clientHandler->leaveChannel(channelName);

    channel.removeClient(clientHandler);

    std::cout << "👋Client " << clientHandler->getNickname() << " has left channel " << channelName << std::endl;
}