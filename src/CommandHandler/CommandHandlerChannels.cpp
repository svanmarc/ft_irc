#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

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

    const bool joinStatus = getServer().joinChannel(clientHandler, channelName);
    std::cout << "Join status: " << joinStatus << std::endl;
    if (joinStatus) {
        Channel &newChannel = getServer().getChannel(channelName);
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
    std::cout << "BEFORE REMOVE: Clients in channel: " << channel.getClients().size() << std::endl;
    channel.removeClient(clientHandler);
    std::cout << "AFTER REMOVE: Clients in channel: " << channel.getClients().size() << std::endl;

    // Supprimer le canal de la liste des canaux du client
    clientHandler->leaveChannel(channelName);
    std::cout << "Client " << clientHandler->getNickname() << " has left channel " << channelName << std::endl;

    // Confirmer au client qu'il a bien quitté le canal
    //:root!root@IP.hosted-by-42lausanne.ch PART #test :hasta la vista Baby
    std::string leaveMessage = ":" + clientHandler->getNickname() + "!" + clientHandler->getUser().getUsername() + "@" + getServer().getServerName();
    leaveMessage += " PART " + channelName + " :Au revoir a jamais";
    MessageHandler::sendMessage(clientHandler->getSocket(),leaveMessage);
    std::cout << "---- Sending leave message: " << leaveMessage << std::endl;
    MessageHandler::sendMessageToAllClientsInChannel(channel, leaveMessage, clientHandler, true);
}

