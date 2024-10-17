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
    for (size_t i = 0; i < channel.getClients().size(); i++) {
        ClientHandler *client = channel.getClients()[i];
        if (client == NULL) {
            std::cerr << "Error: Found a null client pointer." << std::endl;
            continue;
        }
        std::cout << "Sending message to client: " << client->getUser().getNickname() << std::endl;
        MessageHandler::sendMessage(client->getSocket(), message);
    }
}

void MessageHandler::sendNewMemberToChannel(ClientHandler *clientHandler, Channel &channel) {
    //':yourNick JOIN :#test'
    std::string message = clientHandler->getUser().getNickname();
    message += " JOIN ";
    message += ": " + channel.getName();
    message += "\r\n";
    std::cout << "Sending new member message to channel " << message << std::endl;
    sendMessageToAllClientsInChannel(channel, message);
}

void MessageHandler::sendCurrentMemberListToNew(ClientHandler *clientHandler, Channel &channel) {
    //:irc.example.com 353 yourNick = #test :user1 user2 user3
    std::string currentUserString;
    for (size_t i = 0; i < channel.getClients().size(); i++) {
        ClientHandler *client = channel.getClients()[i];
        if (client == NULL) {
            std::cerr << "Error: Found a null client pointer." << std::endl;
            continue;
        }
        currentUserString += client->getUser().getNickname() + " ";
    }
    std::string message = clientHandler->getUser().getNickname();
    message += " = ";
    message += channel.getName();
    message += " :";
    message += currentUserString;
    std::cout << "Sending current member list to new client " << message << std::endl;
    sendResponse(clientHandler, IRCConstants::RPL_NAMREPLY,message);
}

void MessageHandler::sendEndOfNamesList(ClientHandler *clientHandler, Channel &channel)
{
    //:irc.example.com 366 yourNick #test :End of NAMES list
    std::string message = clientHandler->getUser().getNickname();
    message += "" + channel.getName();
    message += " :End of NAMES list";
    std::cout << "Sending End of NAMES list new client " << message << std::endl;
    sendResponse(clientHandler, IRCConstants::RPL_ENDOFNAMES, message);
 }
