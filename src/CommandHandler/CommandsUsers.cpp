#include "CommandHandler.hpp"

void CommandHandler::handleWhois(const std::string &command, ClientHandler *clientHandler) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if ((parts.size() == 1 || !parts[1].find("CAP"))) {
        MessageHandler::MessageWhoisNoGiven(clientHandler);
        return;
    }
    const std::string targetNickname = trim(parts[1]);
    // Chercher le client correspondant au `targetNickname`
    const std::vector<ClientHandler *> &clients = m_server.getClients(); // Récupérer les clients
    bool found = false;
    for (std::vector<ClientHandler *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getNickname() == targetNickname) // Vérifier la correspondance du `nickname`
        {
            found = true;
            MessageHandler::MessageWhois(clientHandler, *it);
            return;
        }
    }
    if (!found)
        MessageHandler::MessageWhoisNotFound(clientHandler, targetNickname);
}
