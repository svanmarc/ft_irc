#include "CommandHandler.hpp"

void CommandHandler::handleWhois(const std::string &command, int clientSocket, ClientHandler *clientHandler) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    std::cout << parts[1] << std::endl;
    if (parts.size() == 2 && (parts[1].empty() || parts[1] == "CAP")) {
        sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "WHOIS :No nickname given");
        return;
    }
    std::string targetNickname = trim(parts[1]);
    // Chercher le client correspondant au `targetNickname`
    const std::vector<ClientHandler *> &clients = m_server.getClients(); // Récupérer les clients
    bool found = false;
    for (std::vector<ClientHandler *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getNickname() == targetNickname) // Vérifier la correspondance du `nickname`
        {
            found = true;
            ClientHandler *targetClient = *it;

            std::string response = "311 " + clientHandler->getNickname() + " ";
            response += targetClient->getNickname() + " ";
            response += targetClient->getUser().getUsername() + " ";
            response += targetClient->getUser().getHostname() + " * :";
            response += targetClient->getUser().getRealname();
            sendResponse(clientSocket, response); // Envoyer la réponse WHOIS au client demandeur
            sendResponse(clientSocket, RPL_WHOISSERVER, targetNickname + " localhost :Server description");
            sendResponse(clientSocket, RPL_ENDOFWHOIS, targetNickname +  " :End of /WHOIS list");
            return;
        }
    }
    if (!found) {
        sendResponse(clientSocket, "WHOIS :No such nickname: " + targetNickname);
    }
}