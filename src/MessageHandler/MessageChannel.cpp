#include "MessageHandler.hpp"

void MessageHandler::sendWelcomeToChannel(ClientHandler *clientHandler, const Channel &channel) {
    std::string topic = channel.getTopic() == "" ? "No topic is set" : channel.getTopic();
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    message += " " + topic;
    std::cout << "FT ------ ending welcome message to channel " << message << std::endl;
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WELCOME, message);
}

/*void MessageHandler::sendMessageToAllClientsInChannel(const Channel &channel, const std::string &message) {
    const std::string messageT = "test";
    std::cout << "Sending message to all clients in channel " << message << std::endl;
    for (std::vector<ClientHandler>::const_iterator it = channel.getClients().begin(); it != channel.getClients().end(); ++it) {
        std::cout << "Sending message to " << (&(*it)) << std::endl;
        sendUserMsg(const_cast<ClientHandler*>(&(*it)), messageT, "");
    }
}
*/
void MessageHandler::sendNewMemberToChannel(ClientHandler *clientHandler, const Channel &channel) {
    std::string message = clientHandler->getUser().getNickname();
    message += " " + channel.getName();
    std::cout << "Sending new member message to channel " << message << std::endl;
    //sendMessageToAllClientsInChannel(channel, message);
}