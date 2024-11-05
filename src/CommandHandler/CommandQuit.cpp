#include "CommandHandler.hpp"

void CommandHandler::handleCap(ClientHandler *clientHandler, const std::string &command) {
    if (command.find("LS") != std::string::npos) {
        MessageHandler::sendCAP(clientHandler);
    }
}

void CommandHandler::handleQuit(ClientHandler *clientHandler, const std::string &command) {
    // Extraire le message de départ après le symbole ':'
    size_t posStartMessage = command.find(':');
    std::string quitMessage = "Client has quit";
    if (posStartMessage != std::string::npos) {
        quitMessage = command.substr(posStartMessage + 1);
    }

    // Récupérer les canaux auxquels le client est connecté
    std::vector<std::string> channels = clientHandler->getChannels();
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
        // Récupérer le canal et envoyer un message à tous les clients connectés à ce canal
        Channel &channel = clientHandler->getServer()->getChannel(*it);
        std::string leaveMessage = ":" + clientHandler->getNickname() + "!" + clientHandler->getUser().getUsername() +
                                   "@" + clientHandler->getUser().getHostname() + " QUIT :" + quitMessage;
        MessageHandler::sendMessageToAllClientsInChannel(channel, leaveMessage, clientHandler, true);

        // Retirer le client du canal
        channel.removeClient(clientHandler);
        clientHandler->leaveChannel(*it);
    }
    std::cout << BLUE << "🛫 Client " << MAGENTA << clientHandler->getNickname() << BLUE << " has quit" << RESET
              << std::endl;

    m_server.handleClientDisconnect(clientHandler->getSocket());
}
