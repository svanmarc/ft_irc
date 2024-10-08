#include "CommandHandler.hpp"

void CommandHandler::handlePrivMsg(const std::string &command, int clientSocket, ClientHandler *clientHandler) {
    std::string::size_type start = command.find("PRIVMSG ");
    if (start == std::string::npos) {
        sendResponse(clientSocket, ERR_UNKNOWNCOMMAND, "Unknown command: " + command);
        return;
    }
    // Extraire le `target`
    start += 8; // Sauter "PRIVMSG "
    std::string::size_type spacePos = command.find(' ', start);
    if (spacePos == std::string::npos) {
        sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "No target specified");
        return;
    }
    std::string target = command.substr(start, spacePos - start);

    // Extraire le message après le symbole ':'
    std::string::size_type messagePos = command.find(':', spacePos);
    if (messagePos == std::string::npos) {
        sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "No message specified");
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
            std::string response = ":" + clientHandler->getNickname() + " PRIVMSG " + target + " :" + message;
            (*it)->sendResponse(response); // Envoie le message au client spécifié
            break;
        }
    }

    if (!found) {
        std::cout << "PRIVMSG from " << clientHandler->getNickname() << " to " << target << ": " << message
                  << std::endl;
        sendResponse(clientSocket, "No such user: " + target);
    }
}
