#include "MessageHandler.hpp"

void MessageHandler::sendWelcomeToChannel(ClientHandler *clientHandler, const Channel &channel) {
    std::string topic = channel.getTopic() == "" ? "No topic is set" : channel.getTopic();
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    message += " " + topic;
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WELCOME, message);
}

void MessageHandler::sendMessageToAllClientsInChannel(Channel &channel, const std::string &message) {
    std::cout << "Sending message to all clients in channel: " << message << channel.getClients().size() << std::endl;
    std::vector<ClientHandler *> clients = channel.getClients();
    for (std::vector<ClientHandler *>::iterator it = clients.begin(); it != clients.end(); ++it) {
        ClientHandler *client = *it;
        if (client == NULL) {
            std::cerr << "Error: Found a null client pointer." << std::endl;
            continue;
        }
        std::cout << "Sending message to client: " << client->getUser().getNickname() << std::endl;
        MessageHandler::sendMessage(client->getSocket(), message);
    }
}

void MessageHandler::sendNewMemberToChannel(ClientHandler *clientHandler, Channel &channel) {
    //:tamm2!root@IP.hosted-by-42lausanne.ch JOIN #1
    std::string message =  ":"+ clientHandler->getUser().getNickname() + "!";
    message += clientHandler->getUser().getUsername() + "@";
    message += clientHandler->getUser().getHostname();
    message += " JOIN";
    message += " " + channel.getName();
    message += "\r\n";
    std::cout << "Sending new member message to channel " << message << std::endl;
    sendMessageToAllClientsInChannel(channel, message);
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
        currentUserString += "@"+ client->getUser().getNickname() + " ";
    }
    std::string message = clientHandler->getUser().getNickname();
    message += " = ";
    message += channel.getName();
    message += " :";
    message += currentUserString;
    std::cout << "Sending current member list to new client " << message << std::endl;
    sendResponse(clientHandler, IRCConstants::RPL_NAMREPLY, message);
}

void MessageHandler::sendEndOfNamesList(ClientHandler *clientHandler, Channel &channel)
{
    //:irc.example.com 366 yourNick #test :End of NAMES list
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    message += " :End of NAMES list";
    std::cout << "Sending End of NAMES list new client " << message << std::endl;
    sendResponse(clientHandler, IRCConstants::RPL_ENDOFNAMES, message);
 }
