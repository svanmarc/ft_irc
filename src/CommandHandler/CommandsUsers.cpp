#include "CommandHandler.hpp"

void CommandHandler::handleWhois(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    std::cout << parts[1] << std::endl;
    if (parts.size() == 2 && (parts[1].empty() || parts[1] == "CAP")) {
        std::string errorMsg = "WHOIS : " + IRCConstants::COLOR_RED
                               + "No nickname given" + IRCConstants::COLOR_RESET;
        MessageHandler::sendResponse(clientHandler, ERR_NEEDMOREPARAMS, errorMsg);
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
