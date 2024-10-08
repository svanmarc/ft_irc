//
// Created by martin on 08/10/2024.
//
#include "CommandHandler.hpp"

void CommandHandler::handleUser(const std::string& command, int clientSocket, ClientHandler* clientHandler) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 5) {
        sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "Manque des paramètres : USER username hostname servername :realname");
        return;
    }

    // Vérifier si l'utilisateur est déjà enregistré
    if (clientHandler->getUser().isRegistered()) {
        sendResponse(clientSocket, ERR_ALREADYREGISTRED, "Vous êtes déjà enregistré petit coquin !");
        return;
    }
    if (clientHandler->getUser().getNickname().empty())
    {
        sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "Vous devez d'abord choisir un pseudo avec la commande NICK");
        return;
    }

    // parts[0] est "USER", on l'ignore
    std::string username = parts[1];
    std::string hostname = parts[2];
    std::string servername = parts[3];
    std::string realname = parts[4];
    std::string nickName = clientHandler->getUser().getNickname();
    clientHandler->getUser().Register(nickName, "", username, realname, hostname, servername);
    completeRegistration(clientSocket, clientHandler);
}

void CommandHandler::handlePass(const std::string &command, int clientSocket, ClientHandler *clientHandler
   ) {
    size_t pos = command.find("PASS ");
    if (pos != std::string::npos && pos + 5 < command.length()) {
        std::string clientPassword = trim(command.substr(pos + 5));

        std::cout << "Client password: " << clientPassword << std::endl;

        if (m_server.authenticate(clientPassword)) {
            clientHandler->setAuthenticated(true);
            clientHandler->resetAttempts(); // Réinitialisez les tentatives en cas de succès
            sendResponse(clientSocket, "Authentication successful! Welcome.");
        } else {
            clientHandler->incrementAttempts(); // Incrémentez les tentatives
            if (clientHandler->getAttempts() >= 3) {
                sendResponse(clientSocket, "Too many failed attempts. Disconnecting...");
                close(clientSocket); // Fermez la connexion après 3 échecs
            } else {
                std::ostringstream oss; // Utilisez un ostringstream pour la conversion
                oss << (3 - clientHandler->getAttempts());
                sendResponse(clientSocket, "Incorrect password! Please try again. Attempts left: " + oss.str());
            }
        }
    } else {
        sendResponse(clientSocket, "Error: No password provided.");
        close(clientSocket);
    }
}


void CommandHandler::completeRegistration(int clientSocket, ClientHandler *clientHandler)
{
    std::string welcomeMsg = ":";
    welcomeMsg += clientHandler->getUser().getServername();
    welcomeMsg += " 001 ";
    welcomeMsg += clientHandler->getNickname();
    welcomeMsg += " :Welcome to the Internet Relay Network ";
    welcomeMsg += clientHandler->getNickname();
    welcomeMsg += "!";
    welcomeMsg += clientHandler->getUser().getUsername();
    welcomeMsg += "@";
    welcomeMsg += clientHandler->getUser().getHostname();
    welcomeMsg += "\r\n";
    std::cout << "Sending welcome message: " << welcomeMsg << std::endl;
    sendResponse(clientSocket, welcomeMsg);
}

void CommandHandler::handleNick(const std::string& command, int clientSocket, ClientHandler* clientHandler) {
    std::string nickname;
    size_t pos = command.find("NICK");
    if (pos != std::string::npos && pos + 5 < command.length()) {
        nickname = trim(command.substr(pos + 5));
    }

    if (nickname.empty()) {
        sendResponse(clientSocket, ERR_UNKNOWNCOMMAND, "No nickname given");
        return;
    }

    clientHandler->getUser().setNickname(nickname);

    std::string welcomeMessage = "Welcome to the Internet Relay Network 2024 ";
    welcomeMessage += nickname;
    sendResponse(clientSocket, RPL_WELCOME, welcomeMessage);
}