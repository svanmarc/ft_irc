#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

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
    const bool joinStatus = getServer().joinChannel(clientHandler, channelName);
    std::cout << "Join status: " << joinStatus << std::endl;
    if (joinStatus) {
        Channel newChannel = getServer().getChannel(channelName);
        MessageHandler::sendWelcomeToChannel(clientHandler, newChannel);
        MessageHandler::sendNewMemberToChannel(clientHandler, newChannel);
        MessageHandler::sendCurrentMemberListToNew(clientHandler, newChannel);
        MessageHandler::sendEndOfNamesList(clientHandler, newChannel);

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

//     // Envoi de la réponse au client
//     MessageHandler::sendResponse(clientHandler, "You have left the channel " + channelName);
// }
