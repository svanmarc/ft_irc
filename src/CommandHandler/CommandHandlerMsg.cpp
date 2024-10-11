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
    const std::string target = parts[1];

    // Extraire le message après le symbole ':'
    const std::string message = parts[3];
    if (message.empty()) {
        MessageHandler::sendErrorNoMessage(clientHandler);
        return;
    }
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