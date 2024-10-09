
#include "CommandHandler.hpp"
#include "MessageHandler.hpp"

void CommandHandler::handleNick(const std::string &command, ClientHandler *clientHandler) {
    // manque la verification si il existe deja !!!!!
    std::string nickname;
    size_t pos = command.find("NICK");
    if (pos != std::string::npos && pos + 5 < command.length()) {
        nickname = trim(command.substr(pos + 5));
    }
    if (nickname.empty()) {
        MessageHandler::sendErrorNoNickNameGiven(clientHandler);
        return;
    }
    std::string oldNickname = clientHandler->getUser().getNickname();
    if (oldNickname.empty()) {
        // Envoyer un message de changement de nickname
        MessageHandler::sendNewNickName(clientHandler, nickname);
    } else {
        // Envoyer un message de bienvenue
        MessageHandler::sendChangeNickName(clientHandler, oldNickname, nickname);
    }
    clientHandler->getUser().setNickname(nickname);
}

void CommandHandler::handleUser(const std::string &command, ClientHandler *clientHandler) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 5) {
        MessageHandler::sendErrorNoUserParams(clientHandler);
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
    // --------- A CHANGER ---------
    std::string username = parts[1];
    std::string hostname = parts[2];
    std::string servername = parts[3];
    std::string realname = parts[4];
    std::string nickName = clientHandler->getUser().getNickname();
    clientHandler->getUser().Register(nickName, "", username, realname, hostname, servername);
    std::cout << "Sending welcome message: ..." << std::endl;
    MessageHandler::sendWelcomeMessage(clientHandler);
}

void CommandHandler::handlePass(const std::string &command, ClientHandler *clientHandler) {
    size_t pos = command.find("PASS ");
    if (pos != std::string::npos && pos + 5 < command.length()) {
        std::string clientPassword = trim(command.substr(pos + 5));
        std::cout << "Client password: " << clientPassword << std::endl;

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
    } else {
        MessageHandler::sendErrorNoPasswordGiven(clientHandler);
        close(clientHandler->getSocket());
    }
}