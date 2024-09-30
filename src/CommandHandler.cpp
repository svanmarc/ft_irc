#include "CommandHandler.hpp"
#include <sys/socket.h>
#include <string.h>
#include <iostream>

void CommandHandler::handleCommand(const std::string& command, int clientSocket, ClientHandler *clientHandler)
{
	    std::cout << "Received command: " << command << std::endl;

    if (command.find("CAP LS") == 0) {
        // Répondre à la commande CAP LS
        const char *capResponse = ":localhost CAP * LS :multi-prefix extended-join account-notify\r\n";
        send(clientSocket, capResponse, strlen(capResponse), 0);
        std::cout << "Handled CAP LS command" << std::endl;

        // Indiquer que le serveur est prêt pour d'autres commandes
        const char *readyResponse = ":localhost 375 :Ready to accept commands\r\n";
        send(clientSocket, readyResponse, strlen(readyResponse), 0);
    } else if (command.find("NICK") == 0) {
        // Gérer la commande NICK
        std::string nickname = command.substr(5);
		clientHandler->setNickname(nickname);
        std::cout << "Nickname set to: " << nickname << std::endl;

        // Répondre avec un message de bienvenue
        std::string welcomeMessage = ":localhost 001 " + nickname + " :Bienvenu dans le Internet Relay Network 2024 " + nickname + " \r\n \x03";
        send(clientSocket, welcomeMessage.c_str(), welcomeMessage.length(), 0);
	} else if (command.find("MODE") == 0) {
		// Gérer la commande MODE
		std::string mode = command.substr(5);
		clientHandler->setUserMode(mode);
		std::string response = ":localhost 324 " + clientHandler->getNickname() + " +" + mode + "\r\n";
		send(clientSocket, response.c_str(), response.length(), 0);
	}   else if (command.find("WHOIS") == 0) {
		// Gérer la commande WHOIS
		std::string response = ":localhost NICK " + clientHandler->getNickname() + "\r\n";
		send(clientSocket, response.c_str(), response.length(), 0);
	}
	else {
        const char *unknownResponse = ":localhost 421 Unknown command\r\n";
		std::cout << "Unknown command" << std::endl;
        send(clientSocket, unknownResponse, strlen(unknownResponse), 0);
    }
}
