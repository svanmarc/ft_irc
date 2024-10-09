#include "CommandHandler.hpp"

void CommandHandler::handlePrivMsg(const std::string &command, ClientHandler *clientHandler) {
    // Extraire le `target`
    std::string::size_type start = command.find("PRIVMSG");
    start += 8; // Sauter "PRIVMSG "
    std::string::size_type spacePos = command.find(' ', start);
    if (spacePos == std::string::npos) {
        MessageHandler::sendErrorNoTarget(clientHandler);
        return;
    }
    std::string target = command.substr(start, spacePos - start);

    // Extraire le message après le symbole ':'
    std::string::size_type messagePos = command.find(':', spacePos);
    if (messagePos == std::string::npos) {
        MessageHandler::sendErrorNoMessage(clientHandler);
        return;
    }
    std::string message = command.substr(messagePos + 1);

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
        std::cout << "PRIVMSG from " << clientHandler->getNickname() << " to " << target << ": " << message
                  << std::endl;
        MessageHandler::sendResponse(clientHandler, "No such user: " + target);
    }
}