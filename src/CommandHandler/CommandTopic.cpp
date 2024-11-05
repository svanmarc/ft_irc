#include "CommandHandler.hpp"
#include "MessageHandler.hpp"

void CommandHandler::handleTopic(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 2) {
        MessageHandler::sendErrorNoSuchChannel(clientHandler, ""); // Erreur si aucun canal n'est spécifié
        return;
    }

    std::string channelName = parts[1];
    std::string newTopic;

    if (parts.size() > 2) {
        newTopic = command.substr(command.find(':') + 1); // Le nouveau sujet est tout ce qui suit ":"
    }

    // Vérifier l'existence du canal
    if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
        MessageHandler::sendErrorNoSuchChannel(clientHandler, channelName);
        return;
    }

    Channel &channel = clientHandler->getServer()->getChannel(channelName);

    // Vérifier si le client est dans le canal
    if (!channel.checkIfClientIsInChannel(clientHandler)) {
        MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
        return;
    }

    // Si un nouveau sujet est défini
    if (!newTopic.empty()) {
        if (channel.getTopicProtection() && !channel.checkIfClientIsOperator(clientHandler)) {
            MessageHandler::sendErrorNotChannelOperator(clientHandler);
            return;
        }
        channel.setTopic(newTopic);
        std::string topicMessage = ":" + clientHandler->getNickname() + " TOPIC " + channelName + " :" + newTopic;
        MessageHandler::sendMessageToAllClientsInChannel(channel, topicMessage, clientHandler, true);
        std::cout << GREEN << "Topic updated for channel " << channelName << " to: " << newTopic << RESET << std::endl;
    } else {
        // Si aucun sujet n'est défini, renvoyer l'état actuel du canal
        if (channel.getTopic().empty()) {
            MessageHandler::sendNoTopic(clientHandler, channel);
        } else {
            MessageHandler::sendTopic(clientHandler, channel);
        }
    }
    std::cout << GREEN << "Topic updated for channel " << channelName << " to: " << newTopic << RESET << std::endl;
}
