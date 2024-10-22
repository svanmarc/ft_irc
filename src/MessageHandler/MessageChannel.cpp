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
    std::cout << clientHandler->getUser().getNickname() << " :Sending message to all clients in channel: " << message
              << std::endl;
    std::vector<ClientHandler *> clients = channel.getClients();
    for (std::vector<ClientHandler *>::iterator it = clients.begin(); it != clients.end(); ++it) {
        ClientHandler *client = *it;
        std::cout << "Sending message to client: " << client->getUser().getNickname() << std::endl;
        if (client == NULL) {
            std::cerr << "Error: Found a null client pointer." << std::endl;
            continue;
        }
        if (client == clientHandler && !sendToSender) {
            continue;
        }
        MessageHandler::sendMessage(client->getSocket(), message);
    }
}

void MessageHandler::sendNewMemberToChannel(ClientHandler *clientHandler, Channel &channel) {
    //: tamm2!root@IP.hosted-by-42lausanne.ch JOIN #1
    std::string message = ":" + clientHandler->getUser().getNickname() + "!";
    message += clientHandler->getUser().getUsername() + "@";
    message += clientHandler->getUser().getHostname();
    message += " JOIN";
    message += " " + channel.getName();
    std::cout << "Sending new member message to channel " << message << std::endl;
    sendMessageToAllClientsInChannel(channel, message, clientHandler, true);
}

void MessageHandler::sendCurrentMemberListToNew(ClientHandler *clientHandler, Channel &channel) {
    //:*.42irc.net 353 tamm2 = #1 :@tamm2
    std::string currentUserString;
    for (size_t i = 0; i < channel.getClients().size(); i++) {
        ClientHandler *client = channel.getClients()[i];
        if (client == NULL) {
            std::cerr << "Error: Found a null client pointer." << std::endl;
            continue;
        }
        currentUserString += "@" + client->getUser().getNickname() + " ";
    }
    std::string message = clientHandler->getUser().getNickname();
    message += " = ";
    message += channel.getName();
    message += " :";
    message += currentUserString;
    std::cout << "Sending current member list to new client " << message << std::endl;
    sendResponse(clientHandler, IRCConstants::RPL_NAMREPLY, message);
}

void MessageHandler::sendEndOfNamesList(ClientHandler *clientHandler, Channel &channel) {
    //: irc.example.com 366 yourNick #test :End of NAMES list
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    message += " :End of NAMES list";
    std::cout << "Sending End of NAMES list new client " << message << std::endl;
    sendResponse(clientHandler, IRCConstants::RPL_ENDOFNAMES, message);
}

void MessageHandler::sendInviteNotification(ClientHandler *invitingClient, ClientHandler *invitedClient,
                                            Channel &channel) {
    // Construire le message d'invitation pour l'utilisateur invité
    std::string inviteMessage = ":" + invitingClient->getNickname() + "!" + invitingClient->getUser().getUsername() +
                                "@ircserver INVITE " + invitedClient->getNickname() + " :" + channel.getName();

    // Envoyer l'invitation à l'utilisateur invité
    sendMessage(invitedClient->getSocket(), inviteMessage);

    // Optionnel : Notifier l'utilisateur qui a envoyé l'invitation
    std::string notifyMessage = "You have invited " + invitedClient->getNickname() + " to " + channel.getName();
    sendMessage(invitingClient->getSocket(), notifyMessage);
}
