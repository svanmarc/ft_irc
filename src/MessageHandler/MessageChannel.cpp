#include "MessageHandler.hpp"

void MessageHandler::sendWelcomeToChannel(ClientHandler *clientHandler, const Channel &channel) {
    std::string topic = channel.getTopic() == "" ? "No topic is set" : channel.getTopic();
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    message += " " + topic;
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WELCOME, message);
}

void MessageHandler::sendMessageToAllClientsInChannel(Channel &channel, const std::string &message,
                                                      ClientHandler *clientHandler, bool sendToSender) {
    std::vector<ClientHandler *> clients = channel.getClients();
    for (std::vector<ClientHandler *>::iterator it = clients.begin(); it != clients.end(); ++it) {
        ClientHandler *client = *it;
        if (client == NULL) {
            std::cerr << RED << "Error: Found a null client pointer." << RESET << std::endl;
            continue;
        }
        if (client == clientHandler && !sendToSender) {
            continue;
        }
        MessageHandler::sendMessage(client->getSocket(), message);
    }
}

void MessageHandler::sendNewMemberToChannel(ClientHandler *clientHandler, Channel &channel) {
    std::string tempMessage = "JOIN " + channel.getName();
    std::string message = messageWithServerPrefixAndSender(clientHandler, tempMessage);
    sendMessageToAllClientsInChannel(channel, message, clientHandler, true);
}

void MessageHandler::sendCurrentMemberListToNew(ClientHandler *clientHandler, Channel &channel) {
    std::string currentUserString = "";
    for (size_t i = 0; i < channel.getClients().size(); i++) {
        ClientHandler *client = channel.getClients()[i];
        if (client == NULL) {
            std::cerr << RED << "Error: Found a null client pointer." << RESET << std::endl;
            continue;
        }
        channel.checkIfClientIsOperator(client) ? currentUserString += "@" : currentUserString += "";
        currentUserString += client->getUser().getNickname() + " ";
    }
    std::string message = clientHandler->getUser().getNickname();
    message += " = ";
    message += channel.getName();
    message += " :";
    message += currentUserString;
    sendResponse(clientHandler, IRCConstants::RPL_NAMREPLY, message);
}

void MessageHandler::sendEndOfNamesList(ClientHandler *clientHandler, Channel &channel) {
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    message += " :End of NAMES list";
    sendResponse(clientHandler, IRCConstants::RPL_ENDOFNAMES, message);
}

void MessageHandler::sendInviteNotification(ClientHandler *invitingClient, ClientHandler *invitedClient,
                                            Channel &channel) {
    // Construire le message d'invitation pour l'utilisateur invité
    std::string tempMessage = "INVITE " + invitedClient->getNickname() + " :" + channel.getName();
    std::string inviteMessage = messageWithServerPrefixAndSender(invitingClient, tempMessage);
    // Envoyer l'invitation à l'utilisateur invité
    sendMessage(invitedClient->getSocket(), inviteMessage);

    // Optionnel : Notifier l'utilisateur qui a envoyé l'invitation
    std::string notifyMessage = "You have invited " + invitedClient->getNickname() + " to " + channel.getName();
    sendMessage(invitingClient->getSocket(), notifyMessage);
    std::cout << BLUE << "📩 " << MAGENTA << invitingClient->getNickname() << BLUE << " has invited " << MAGENTA
              << invitedClient->getNickname() << BLUE << " to " << MAGENTA << channel.getName() << RESET << std::endl;
}


void MessageHandler::sendChannelModes(ClientHandler *clientHandler, Channel &channel, const std::string &modeSign,
                                      const std::string mode) {

    // Construction du message sans crochets
    std::string modeMessage = "MODE " + channel.getName() + " " + modeSign + mode;

    // Ajouter les paramètres pour les modes `k` et `l`
    modeMessage += " :";
    if (mode == "k")
        modeMessage += channel.getPassword();
    if (mode == "l") {
        std::stringstream ss;
        ss << channel.getUserLimit();
        modeMessage += ss.str();
    }
    modeMessage = messageWithServerPrefixAndSender(clientHandler, modeMessage);
    sendMessageToAllClientsInChannel(channel, modeMessage, clientHandler, false);
}


void MessageHandler::sendOpMode(ClientHandler *clientHandler, ClientHandler *targetClient, Channel &channel,
                                const std::string &modeSign) {
    std::string modeMessage = "MODE " + channel.getName() + " " + modeSign + "o " + targetClient->getNickname();
    modeMessage = messageWithServerPrefixAndSender(clientHandler, modeMessage);
    sendMessageToAllClientsInChannel(channel, modeMessage, clientHandler, true);
}

void MessageHandler::sendChannelList(ClientHandler *clientHandler, Channel &channel) {
    std::stringstream ss;
    ss << clientHandler->getNickname() + " " + channel.getName() << " "
       << static_cast<int>(channel.getNumberOfClients()) << " [modes ::" << channel.getModes()
       << "] :" + channel.getTopic();
    sendResponse(clientHandler, 322, ss.str());
}

void MessageHandler::sendEndOfList(ClientHandler *clientHandler) {
    sendResponse(clientHandler, 323,
                 clientHandler->getNickname() + ": " + IRCConstants::COLOR_YELLOW + "End of list" +
                         IRCConstants::COLOR_RESET);
}

void MessageHandler::sendStartOfList(ClientHandler *clientHandler) {
    sendResponse(clientHandler, 321, clientHandler->getNickname() + " Channel :Users Name");
}

void MessageHandler::sendChannelsList(ClientHandler *clientHandler) {
    sendStartOfList(clientHandler);
    std::vector<Channel> channels = clientHandler->getServer()->getChannels();
    sendResponse(clientHandler, 322,
                 clientHandler->getNickname() + ": " + IRCConstants::COLOR_GREEN + "List of Channels" +
                         IRCConstants::COLOR_RESET);
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel channel = *it;
        sendChannelList(clientHandler, channel);
    }
    sendEndOfList(clientHandler);
}

void MessageHandler::sendMessageToClient(ClientHandler *clientHandler, const std::string &message) {
    std::string msgToSend = message;
    msgToSend = messageWithServerPrefixAndSender(clientHandler, msgToSend);
    sendMessage(clientHandler->getSocket(), msgToSend);
}

void MessageHandler::sendTopic(ClientHandler *clientHandler, const Channel &channel) {
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    message += " :" + channel.getTopic();
    sendResponse(clientHandler, IRCConstants::RPL_TOPIC, message);
}

void MessageHandler::sendNoTopic(ClientHandler *clientHandler, const Channel &channel) {
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    message += " :No topic is set";
    sendResponse(clientHandler, IRCConstants::RPL_NOTOPIC, message);
}
