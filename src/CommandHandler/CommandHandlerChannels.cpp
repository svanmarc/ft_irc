#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

// void CommandHandler::handleJoinChannel(ClientHandler *clientHandler, const std::string &command) {
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
//         Channel &newChannel = getServer().getChannel(channelName);
//         MessageHandler::sendWelcomeToChannel(clientHandler, newChannel);
//         MessageHandler::sendNewMemberToChannel(clientHandler, newChannel);
//         MessageHandler::sendCurrentMemberListToNew(clientHandler, newChannel);
//         MessageHandler::sendEndOfNamesList(clientHandler, newChannel);
//     }
// }

void CommandHandler::handleJoinChannel(ClientHandler *clientHandler, const std::string &command) {
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

    try {
        Channel &channel = clientHandler->getServer()->getChannel(channelName);
        std::cout << "Checking if channel is invite-only..." << std::endl;
        if (channel.getInviteOnly()) {
            std::cout << "Channel is invite-only. Checking if client is invited..." << std::endl;
            if (!channel.isClientInvited(clientHandler)) {
                MessageHandler::sendErrorInviteOnly(clientHandler, channelName);
                std::cout << "Client is not invited. Sending error message." << std::endl;
                return;
            } else {
                std::cout << "Client is invited. Allowing join." << std::endl;
            }
        }
    } catch (const std::runtime_error &e) {
        std::cout << "Channel not found. Creating new channel." << std::endl;
        // Le canal n'existe pas, nous allons le créer
    }

    const bool joinStatus = clientHandler->getServer()->joinChannel(clientHandler, channelName);
    std::cout << "Join status: " << joinStatus << std::endl;
    if (joinStatus) {
        Channel &newChannel = clientHandler->getServer()->getChannel(channelName);
        clientHandler->confirmJoinChannel(channelName);
        MessageHandler::sendWelcomeToChannel(clientHandler, newChannel);
        MessageHandler::sendNewMemberToChannel(clientHandler, newChannel);
        MessageHandler::sendCurrentMemberListToNew(clientHandler, newChannel);
        MessageHandler::sendEndOfNamesList(clientHandler, newChannel);
    }
}

void CommandHandler::handlePart(ClientHandler *clientHandler, const std::string &command) {
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

    // Retirer le client du canal au niveau du serveur
    Channel &channel = clientHandler->getServer()->getChannel(channelName);
    channel.removeClient(clientHandler);
    // Supprimer le canal de la liste des canaux du client
    clientHandler->leaveChannel(channelName);
    std::cout << "👋Client " << clientHandler->getNickname() << " has left channel " << channelName << std::endl;

    // Confirmer au client qu'il a bien quitté le canal
    std::string leaveMessage = ":" + clientHandler->getNickname() + "!" + clientHandler->getUser().getUsername() + "@" +
                               getServer().getServerName();
    leaveMessage += " PART " + channelName + " :Au revoir a jamais";
    MessageHandler::sendMessage(clientHandler->getSocket(), leaveMessage);
    std::cout << "---- Sending leave message: " << leaveMessage << std::endl;
    MessageHandler::sendMessageToAllClientsInChannel(channel, leaveMessage, clientHandler, true);
}
