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
    if (clientHandler->user.isRegistered()) {
        sendResponse(clientSocket, ERR_ALREADYREGISTRED, "Vous êtes déjà enregistré petit coquin !");
        return;
    }

    // parts[0] est "USER", on l'ignore
    std::string username = parts[1];
    std::string hostname = parts[2];
    std::string servername = parts[3];
    std::string realname = parts[4];
    std::string nickName = clientHandler->user.getNickname();

    // Mettre à jour les informations de l'utilisateur
    clientHandler->user.setUsername(username);
    clientHandler->user.setHostname(hostname);
    clientHandler->user.setRealname(realname);

    if (!clientHandler->user.getNickname().empty()) {
        completeRegistration(clientSocket, clientHandler);
    }
    else {
        sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "Vous devez d'abord choisir un pseudo avec la commande NICK");
    }
}

void CommandHandler::completeRegistration(int clientSocket, ClientHandler* clientHandler) {
    // Create welcome messages
    std::string welcomeMsg = ":";
    welcomeMsg += clientHandler->user.getServername();
    welcomeMsg += " 001 ";
    welcomeMsg += clientHandler->user.getNickname();
    welcomeMsg += " :Welcome to the Internet Relay Network ";
    welcomeMsg += clientHandler->user.getNickname();
    welcomeMsg += "!";
    welcomeMsg += clientHandler->user.getUsername();
    welcomeMsg += "@";
    welcomeMsg += clientHandler->user.getHostname();
    welcomeMsg += "\r\n";
    std::cout << "Sending welcome message: " << welcomeMsg << std::endl;
    sendResponse(clientSocket, welcomeMsg);
    clientHandler->user.setIsRegistered(true);
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

    clientHandler->user.setNickname(nickname);

    std::string welcomeMessage = "Welcome to the Internet Relay Network 2024 ";
    welcomeMessage += nickname;
    sendResponse(clientSocket, RPL_WELCOME, welcomeMessage);
}