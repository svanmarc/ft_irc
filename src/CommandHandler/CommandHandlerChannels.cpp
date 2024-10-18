#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

// void CommandHandler::handleJoinChannel(const std::string &command, ClientHandler *clientHandler) {
//     std::vector<std::string> parts;
//     splitCommand(command, parts);
//     if (parts.size() < 2) {
//         MessageHandler::sendErrorJoinChannel(clientHandler, "");
//         return;
//     }
//     std::string channelName = trim(parts[1]);
//     if (channelName[0] != '#') {
//         MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
//         return;
//     }
//     const bool joinStatus = getServer().joinChannel(clientHandler, channelName);
//     std::cout << "Join status: " << joinStatus << std::endl;
//     if (joinStatus) {
//         Channel newChannel = getServer().getChannel(channelName);
//         MessageHandler::sendWelcomeToChannel(clientHandler, newChannel);
//         MessageHandler::sendNewMemberToChannel(clientHandler, newChannel);
//         MessageHandler::sendCurrentMemberListToNew(clientHandler, newChannel);
//         MessageHandler::sendEndOfNamesList(clientHandler, newChannel);

//     } else {
//         MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
//     }
// }

// void CommandHandler::handleJoinChannel(const std::string &command, ClientHandler *clientHandler) {
//     std::vector<std::string> parts;
//     splitCommand(command, parts);
//     if (parts.size() < 2) {
//         MessageHandler::sendErrorJoinChannel(clientHandler, "");
//         return;
//     }
//     std::string channelName = trim(parts[1]);
//     if (channelName[0] != '#') {
//         MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
//         return;
//     }
//     // Vérifie si le client peut joindre le canal
//     const bool joinStatus = getServer().joinChannel(clientHandler, channelName);
//     std::cout << "Join status: " << joinStatus << std::endl;

//     if (joinStatus) {
//         // Utiliser une référence pour éviter la copie du canal
//         Channel &newChannel = getServer().getChannel(channelName); // référence au lieu d'une copie
//         MessageHandler::sendWelcomeToChannel(clientHandler, newChannel);
//         MessageHandler::sendNewMemberToChannel(clientHandler, newChannel);
//         MessageHandler::sendCurrentMemberListToNew(clientHandler, newChannel);
//         MessageHandler::sendEndOfNamesList(clientHandler, newChannel);
//     } else {
//         MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
//     }
// }

void CommandHandler::handleJoinChannel(const std::string &command, ClientHandler *clientHandler) {
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

    // Ajout d'une vérification pour éviter la duplication
    if (clientHandler->isInChannel(channelName)) {
        std::cout << "[DEBUG] Client already in channel: " << channelName << std::endl;
        return;
    }

    // Vérifie si le client peut joindre le canal
    const bool joinStatus = getServer().joinChannel(clientHandler, channelName);
    std::cout << "[DEBUG] Join status: " << joinStatus << std::endl;

    if (joinStatus) {
        Channel &newChannel = getServer().getChannel(channelName);
        MessageHandler::sendWelcomeToChannel(clientHandler, newChannel);
        MessageHandler::sendNewMemberToChannel(clientHandler, newChannel);
        MessageHandler::sendCurrentMemberListToNew(clientHandler, newChannel);
        MessageHandler::sendEndOfNamesList(clientHandler, newChannel);

        // Ajout pour afficher l'état des clients après un JOIN
        newChannel.printAllNicknamesInChannel();
    } else {
        MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
    }
}


// void CommandHandler::handlePart(const std::string &command, ClientHandler *clientHandler) {
//     std::vector<std::string> parts;
//     splitCommand(command, parts);

//     if (parts.size() < 2) {
//         MessageHandler::sendErrorNoTarget(clientHandler);
//         return;
//     }

//     std::string channelName = trim(parts[1]);

//     if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
//         MessageHandler::sendErrorNoSuchNick(clientHandler, channelName);
//         return;
//     }

//     Channel &channel = clientHandler->getServer()->getChannel(channelName);
//     channel.removeClient(clientHandler);

//     std::string leaveMessage = clientHandler->getNickname() + " has left the channel " + channelName;

//     // Envoi du message à tous les clients du canal
//     MessageHandler::sendMessageToAllClientsInChannel(channel, leaveMessage);

//     // supprssion du canal de la liste des canaux du client
//     clientHandler->leaveChannel(channelName);

//     // Envoi de la réponse au client
//     MessageHandler::sendResponse(clientHandler, "You have left the channel " + channelName);
// }

// void CommandHandler::handlePart(const std::string &command, ClientHandler *clientHandler) {
//     std::vector<std::string> parts;
//     splitCommand(command, parts);

//     if (parts.size() < 2) {
//         MessageHandler::sendErrorNoTarget(clientHandler);
//         return;
//     }

//     std::string channelName = trim(parts[1]);

//     if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
//         MessageHandler::sendErrorNoSuchNick(clientHandler, channelName);
//         return;
//     }

//     // Retirer le client du canal au niveau du serveur
//     Channel &channel = clientHandler->getServer()->getChannel(channelName);
//     std::cout << "BEFORE REMOVE: Clients in channel: " << channel.getClients().size() << std::endl;
//     channel.removeClient(clientHandler);
//     std::cout << "AFTER REMOVE: Clients in channel: " << channel.getClients().size() << std::endl;

//     // Envoi d'un message à tous les autres clients du canal pour signaler le départ
//     std::string leaveMessage = clientHandler->getNickname() + " has left the channel " + channelName;
//     MessageHandler::sendMessageToAllClientsInChannel(channel, leaveMessage);

//     // Supprimer le canal de la liste des canaux du client
//     clientHandler->leaveChannel(channelName);
//     std::cout << "Client " << clientHandler->getNickname() << " has left channel " << channelName << std::endl;

//     // Confirmer au client qu'il a bien quitté le canal
//     MessageHandler::sendResponse(clientHandler, "You have left the channel " + channelName);
// }

void CommandHandler::handlePart(const std::string &command, ClientHandler *clientHandler) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 2) {
        MessageHandler::sendErrorNoTarget(clientHandler);
        return;
    }

    std::string channelName = trim(parts[1]);

    if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
        MessageHandler::sendErrorNoSuchNick(clientHandler, channelName);
        return;
    }

    Channel *channel = clientHandler->getServer()->getChannel(channelName);
    if (!channel->checkIfClientIsInChannel(clientHandler)) {
        MessageHandler::sendErrorNoSuchNick(clientHandler, channelName);
        return;
    }

    // Message à tous les clients du canal indiquant que l'utilisateur a quitté
    std::string partMessage = ":" + clientHandler->getNickname() + "!" + clientHandler->getUser().getUsername() + "@" +
                              "hostname" + " PART " + channelName + " :hasta la vista Baby";
    MessageHandler::sendMessageToAllClientsInChannel(*channel, partMessage);

    // Message envoyé au client qui quitte
    MessageHandler::sendMessage(clientHandler->getSocket(), partMessage);

    // Retirer le client du canal
    clientHandler->getServer()->removeClientFromChannel(clientHandler, channelName);

    // Suppression du canal si vide
    if (channel->getClients().empty()) {
        clientHandler->getServer()->removeChannel(channelName);
    }

    std::cout << "Client " << clientHandler->getNickname() << " has left channel " << channelName << std::endl;
}
