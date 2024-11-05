
#include "CommandHandler.hpp"
#include "MessageHandler.hpp"

void CommandHandler::handleUser(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 5) {
        MessageHandler::sendErrorModeParams(clientHandler);
        return;
    }
    // Vérifier si l'utilisateur est déjà enregistré
    if (clientHandler->getUser().isRegistered()) {
        MessageHandler::sendErrorAlreadyRegistered(clientHandler);
        return;
    }
    // Vérifier si le nickname est déjà défini
    if (clientHandler->getUser().getNickname().empty()) {
        MessageHandler::sendErrorNotNickNameGiven(clientHandler);
        return;
    }
    std::string username = parts[1];
    std::string hostname = parts[2];
    std::string servername = parts[3];
    std::string realname = parts[4];
    std::string nickName = clientHandler->getUser().getNickname();
    clientHandler->getUser().Register(nickName, "", username, realname, hostname, servername);
    MessageHandler::sendWelcomeMessage(clientHandler);
}

void CommandHandler::handlePing(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 2) {
        MessageHandler::sendErrorNoPingParams(clientHandler);
        return;
    }
    const std::string serverName = parts[1];
    MessageHandler::sendPong(clientHandler);
}

void CommandHandler::handlePass(ClientHandler *clientHandler, const std::string &command) {
    if (clientHandler->isAuthenticated()) {
        return;
    }
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 2 || parts[1].empty()) {
        MessageHandler::sendErrorNoPasswordGiven(clientHandler);
        return;
    }
    const std::string clientPassword = trim(parts[1]);
    if (m_server.authenticate(clientPassword)) {
        clientHandler->setAuthenticated(true);
        clientHandler->resetAttempts(); // Réinitialisez les tentatives en cas de succès
        MessageHandler::sendAuthentificationSuccess(clientHandler);
    } else {
        clientHandler->incrementAttempts(); // Incrémentez les tentatives
        if (clientHandler->getAttempts() >= 3) {
            // Fermez la connexion après 3 échecs
            MessageHandler::sendErrorTooManyAttempts(clientHandler);
            close(clientHandler->getSocket());
        } else {
            std::ostringstream oss; // Utilisez un ostringstream pour la conversion
            oss << (3 - clientHandler->getAttempts());
            MessageHandler::sendErrorIncorrectPassword(clientHandler, oss.str());
        }
    }
}
