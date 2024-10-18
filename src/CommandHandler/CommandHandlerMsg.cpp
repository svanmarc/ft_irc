#include "CommandHandler.hpp"

void CommandHandler::handlePrivMsg(const std::string &command, ClientHandler *clientHandler) {
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
    std::string cmd = parts[0];

    // Extraire le message après le symbole ':'
    size_t posStartMessg = command.find(':');
    if (posStartMessg == std::string::npos) {
        MessageHandler::sendErrorNoMessage(clientHandler);
        return;
    }
    std::string message = command.substr(posStartMessg + 1);
    if (target[0] == '#')
    {
        // Envoi du message à tous les clients dans le canal spécifié
        Channel channel = m_server.getChannel(target);
        if (!channel.checkIfClientIsInChannel(clientHandler)) {
            MessageHandler::sendErrorNotInChannel(clientHandler, target);
            return;
        }
        //:tamm2!root@IP.hosted-by-42lausanne.ch PRIVMSG #d :slugt
        std::cout << "Sending message to channel " << target << ": " << message << std::endl;
        std::string formattedMessage = ":" + clientHandler->getNickname();
        formattedMessage += "!" + clientHandler->getUser().getUsername() + "@" + clientHandler->getUser().getHostname();
        formattedMessage += " " + cmd + " " + channel.getName() + " :" + message + "\r\n";
        MessageHandler::sendMessageToAllClientsInChannel(channel, formattedMessage, clientHandler, false);
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
            MessageHandler::sendMessageToUser(clientHandler, *it, message, cmd == "NOTICE");
            break;
        }
    }

    if (!found) {
        std::cout << "Error sending message: No such user " << target << std::endl;
        MessageHandler::sendErrorNoSuchUser(clientHandler, target);
    }
}
