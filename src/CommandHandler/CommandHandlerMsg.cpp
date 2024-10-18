#include "CommandHandler.hpp"

void CommandHandler::handlePrivMsg(const std::string &command, ClientHandler *clientHandler) {
    // Extraire le `target`
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 3 || parts[1].empty()) {
        MessageHandler::sendErrorNoNickNameGiven(clientHandler);
        return;
    }
    if (parts[1].empty()) {
        MessageHandler::sendErrorNoTarget(clientHandler);
        return;
    }
    std::string target = parts[1];

    // Extraire le message après le symbole ':'
    const std::string message = parts[3];
    if (message.empty()) {
        MessageHandler::sendErrorNoMessage(clientHandler);
        return;
    }
    if (target[0] == '#')
    {
        // Envoi du message à tous les clients dans le canal spécifié
        //:tamm2!root@IP.hosted-by-42lausanne.ch PRIVMSG #d :slugt
        std::cout << "Sending message to channel " << target << ": " << message << std::endl;
        Channel channel = m_server.getChannel(target);
        std::string formattedMessage = ":" + clientHandler->getNickname();
        formattedMessage += "!" + clientHandler->getUser().getUsername() + "@" + clientHandler->getUser().getHostname();
        formattedMessage += " PRIVMSG " + channel.getName() + " :" + message + "\r\n";
        MessageHandler::sendMessageToAllClientsInChannel(channel, formattedMessage);
        return;
    }

    // Envoi du message au destinataire spécifié
    bool found = false;
    const std::vector<ClientHandler *> &clients =
            m_server.getClients(); // Utiliser le getter de `Server` pour récupérer les clients

    for (std::vector<ClientHandler *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getNickname() == target) // Utiliser `getNickname()` pour accéder au nom d'utilisateur
        {
            found = true;
            MessageHandler::sendMessageToUser(clientHandler, *it, message);
            break;
        }
    }

    if (!found) {
        std::cout << "Error sending message: No such user " << target << std::endl;
        MessageHandler::sendErrorNoSuchUser(clientHandler, target);
    }
}