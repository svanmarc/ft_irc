// #include "CommandHandler.hpp"

// void CommandHandler::handleInvite(ClientHandler *clientHandler, const std::string &command) {
//     std::vector<std::string> parts;
//     splitCommand(command, parts);

//     if (parts.size() < 2) {
//         MessageHandler::sendErrorNoTarget(clientHandler);
//         return;
//     }

//     std::string target = trim(parts[1]);
//     std::string param = (parts.size() > 2) ? trim(parts[2]) : "";

//     std::string channelName;
//     if (!param.empty()) {
//         if (param[0] != '#' || !clientHandler->getServer()->checkIfChannelExists(param)) {
//             MessageHandler::sendErrorNoSuchChannel(clientHandler, param);
//             return;
//         }
//         channelName = param;
//     } else {
//         if (clientHandler->getChannels().empty()) {
//             MessageHandler::sendErrorNotInChannel(clientHandler, "current");
//             return;
//         }
//         channelName = clientHandler->getChannels().back();
//     }

//     Channel &channel = clientHandler->getServer()->getChannel(channelName);
//     if (!channel.checkIfClientIsInChannel(clientHandler)) {
//         MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
//         return;
//     }

//     ClientHandler *targetClient = NULL;
//     if (clientHandler->getServer()->getUserByNickname(target, targetClient)) {
//         std::string errormsg = "INVITE " + target + " " + channelName;
//         if (!channel.checkIfClientIsOperator(clientHandler)) {
//             errormsg += " :You're not a channel operator";
//             MessageHandler::sendMessageToClient(clientHandler, errormsg);
//             return;
//         }
//         if (channel.checkIfClientIsInChannel(targetClient)) {
//             errormsg += " :is already on channel";
//             MessageHandler::sendMessageToClient(clientHandler, errormsg);
//             return;
//         }

//         if (channel.isClientInvited(targetClient)) {
//             errormsg += " :is already invited";
//             MessageHandler::sendMessageToClient(clientHandler, errormsg);
//             return;
//         }

//         channel.inviteClient(targetClient);
//         MessageHandler::sendInviteNotification(clientHandler, targetClient, channel);
//         std::string message = ":" + clientHandler->getNickname() + " INVITE " + target + " " + channelName + "\r\n";
//         MessageHandler::sendMessageToAllClientsInChannel(channel, message, clientHandler, targetClient);
//     } else {
//         std::string errormsg = "INVITE " + target;
//         errormsg += " :No such nick/channel";
//         MessageHandler::sendMessageToClient(clientHandler, errormsg);
//     }
// }

#include "CommandHandler.hpp"

void CommandHandler::handleInvite(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 2) {
        MessageHandler::sendErrorNoTarget(clientHandler);
        std::cout << "Debug: Invite command failed - No target specified." << std::endl;
        return;
    }

    std::string target = trim(parts[1]);
    std::string param = (parts.size() > 2) ? trim(parts[2]) : "";

    std::string channelName;
    if (!param.empty()) {
        if (param[0] != '#' || !clientHandler->getServer()->checkIfChannelExists(param)) {
            MessageHandler::sendErrorNoSuchChannel(clientHandler, param);
            std::cout << "Debug: Invite command failed - Channel " << param << " does not exist." << std::endl;
            return;
        }
        channelName = param;
    } else {
        if (clientHandler->getChannels().empty()) {
            MessageHandler::sendErrorNotInChannel(clientHandler, "current");
            std::cout << "Debug: Invite command failed - Client is not in any channel." << std::endl;
            return;
        }
        channelName = clientHandler->getChannels().back();
    }

    std::cout << "Debug: Invite command - Target: " << target << ", Channel: " << channelName << std::endl;

    Channel &channel = clientHandler->getServer()->getChannel(channelName);
    if (!channel.checkIfClientIsInChannel(clientHandler)) {
        MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
        std::cout << "Debug: Invite command failed - Client is not in channel " << channelName << std::endl;
        return;
    }

    ClientHandler *targetClient = NULL;
    if (clientHandler->getServer()->getUserByNickname(target, targetClient)) {
        std::cout << "Debug: Found target client " << targetClient->getNickname() << std::endl;
        if (!channel.checkIfClientIsOperator(clientHandler)) {
            std::string errormsg = "INVITE " + target + " :" + channelName + " :You're not a channel operator";
            MessageHandler::sendMessageToClient(clientHandler, errormsg);
            std::cout << "Debug: Invite command failed - Client is not a channel operator." << std::endl;
            return;
        }
        if (channel.checkIfClientIsInChannel(targetClient)) {
            std::string errormsg = "INVITE " + target + " :" + channelName + " :is already on channel";
            MessageHandler::sendMessageToClient(clientHandler, errormsg);
            std::cout << "Debug: Invite command failed - Target client is already in the channel." << std::endl;
            return;
        }

        if (channel.isClientInvited(targetClient)) {
            std::string errormsg = "INVITE " + target + " :" + channelName + " :is already invited";
            MessageHandler::sendMessageToClient(clientHandler, errormsg);
            std::cout << "Debug: Invite command failed - Target client is already invited." << std::endl;
            return;
        }

        channel.inviteClient(targetClient);
        MessageHandler::sendInviteNotification(clientHandler, targetClient, channel);
        std::string message = ":" + clientHandler->getNickname() + " INVITE " + target + " " + channelName + "\r\n";
        MessageHandler::sendMessageToAllClientsInChannel(channel, message, clientHandler, targetClient);
        std::cout << "Debug: Invite command successful - " << target << " invited to " << channelName << std::endl;
    } else {
        std::string errormsg = "INVITE " + target + " :No such nick/channel";
        MessageHandler::sendMessageToClient(clientHandler, errormsg);
        std::cout << "Debug: Invite command failed - No such nick/channel for target " << target << std::endl;
    }
}
