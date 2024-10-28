#include "CommandHandler.hpp"
#include "MessageHandler.hpp"

void CommandHandler::handleTopic(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 2) {
        MessageHandler::sendErrorNoSuchChannel(clientHandler, "");  // Erreur si aucun canal n'est spécifié
        return;
    }

    std::string channelName = parts[1];
    std::string newTopic;

    if (parts.size() > 2) {
        newTopic = command.substr(command.find(':') + 1);  // Le nouveau sujet est tout ce qui suit ":"
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

    // Si aucun sujet n'est fourni, retourne le sujet actuel
    if (newTopic.empty()) {
        MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_TOPIC, channel.getTopic());
        return;
    }

    // Vérifier le mode `+t` pour les permissions de modification
    if (channel.getTopicProtection() && !channel.checkIfClientIsOperator(clientHandler)) {
        MessageHandler::sendErrorNotChannelOperator(clientHandler);
        return;
    }

    // Mettre à jour le sujet et informer le canal
    channel.setTopic(newTopic);
    std::string topicMessage = ":" + clientHandler->getNickname() + " TOPIC " + channelName + " :" + newTopic;
    MessageHandler::sendMessageToAllClientsInChannel(channel, topicMessage, clientHandler, true);
    std::cout << "Topic updated for channel " << channelName << " to: " << newTopic << std::endl;
}
