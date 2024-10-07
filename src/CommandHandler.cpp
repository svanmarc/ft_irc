#include "CommandHandler.hpp"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <sys/socket.h>
#include <string.h>

const std::string CommandHandler::WELCOME_MESSAGE = "Welcome to the Internet Relay Network 2024 ";

void CommandHandler::handleCommand(const std::string &command, int clientSocket, ClientHandler *clientHandler)
{
	try
	{
		std::cout << "Received command: " << command << std::endl;

		std::string cmd = parseCommand(command);
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);

		if (cmd == "CAP")
		{
			handleCap(command, clientSocket);
		}
		else if (cmd == "NICK")
		{
			handleNick(command, clientSocket, clientHandler);
		}
		else if (cmd == "USER")
		{
			handleUser(command, clientSocket, clientHandler);
		}
		else if (cmd == "PASS")
		{
			handlePass(command, clientSocket, clientHandler);
		}
		else
		{
			sendResponse(clientSocket, ERR_UNKNOWNCOMMAND,
						 std::string("Unknown command: ") + cmd);
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error handling command: " << e.what() << std::endl;
		sendResponse(clientSocket, ERR_UNKNOWNCOMMAND,
					 "Error processing command");
	}
}

std::string CommandHandler::parseCommand(const std::string &fullCommand)
{
	std::istringstream iss(fullCommand);
	std::string cmd;
	iss >> cmd;
	return cmd;
}

void CommandHandler::handleCap(const std::string &command, int clientSocket)
{
	if (command.find("LS") != std::string::npos)
	{
		sendResponse(clientSocket, RPL_CAPLS,
					 "CAP * LS :multi-prefix extended-join account-notify");

		sendResponse(clientSocket, RPL_CAPLS,
					 "Ready to accept commands");
	}
}

void CommandHandler::handleNick(const std::string &command, int clientSocket, ClientHandler *clientHandler)
{
	std::string nickname;
	size_t pos = command.find("NICK");
	if (pos != std::string::npos && pos + 5 < command.length())
	{
		nickname = trim(command.substr(pos + 5));
	}

	if (nickname.empty())
	{
		sendResponse(clientSocket, ERR_UNKNOWNCOMMAND,
					 "No nickname given");
		return;
	}

	clientHandler->m_user.setNickname(nickname);

	std::string welcomeMessage = "Welcome to the Internet Relay Network 2024 ";
	welcomeMessage += nickname;
	sendResponse(clientSocket, RPL_WELCOME, welcomeMessage);
}

void CommandHandler::handleUser(const std::string &command, int clientSocket, ClientHandler *clientHandler)
{
	std::vector<std::string> parts;
	splitCommand(command, parts);
	if (parts.size() < 5)
	{
		sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "Manque des paramètres : USER username hostname servername :realname");
		return;
	}

	// Vérifier si l'utilisateur est déjà enregistré
	if (clientHandler->m_user.isRegistered())
	{
		sendResponse(clientSocket, ERR_ALREADYREGISTRED, "Vous êtes déjà enregistré !");
		return;
	}

	// parts[0] est "USER", on l'ignore
	std::string username = parts[1];
	std::string hostname = parts[2];
	std::string servername = parts[3];
	std::string realname = parts[4];

	clientHandler->m_user.setUsername(username);
	clientHandler->m_user.setHostname(hostname);
	clientHandler->m_user.setRealname(realname);

	if (!clientHandler->m_user.getNickname().empty())
	{
		completeRegistration(clientSocket, clientHandler);
	}
	else
	{
		sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "Vous devez d'abord choisir un pseudo avec la commande NICK");
	}
}

void CommandHandler::handlePass(const std::string &command, int clientSocket, ClientHandler *clientHandler)
{
	size_t pos = command.find("PASS ");
	if (pos != std::string::npos && pos + 5 < command.length())
	{
		std::string clientPassword = trim(command.substr(pos + 5));
		clientHandler->m_user.setPassword("pw2"); // Définir un mot de passe par défaut pour l'exemple

		if (clientPassword == clientHandler->m_user.getPassword())
		{ // Compare avec le mot de passe défini pour l'utilisateur
			clientHandler->m_user.setAuthenticated(true);
			sendResponse(clientSocket, "Authentication successful! Welcome.");
		}
		else
		{
			sendResponse(clientSocket, "Incorrect password!");
			close(clientSocket); // Ferme le client si le mot de passe est incorrect
		}
	}
	else
	{
		sendResponse(clientSocket, "Error: No password provided.");
		close(clientSocket);
	}
}

void CommandHandler::sendResponse(int clientSocket, int code, const std::string &message)
{
	std::string response = ":localhost ";
	std::ostringstream oss;
	oss << code;
	response += oss.str();
	response += " " + message + "\r\n";
	std::cout << "Sending response: " << response << std::endl;
	send(clientSocket, response.c_str(), response.length(), 0);
}

void CommandHandler::sendResponse(int clientSocket, const std::string &message)
{
	std::string response = ":localhost ";
	std::ostringstream oss;
	response += oss.str();
	response += " " + message + "\r\n";
	send(clientSocket, response.c_str(), response.length(), 0);
}

void CommandHandler::completeRegistration(int clientSocket, ClientHandler *clientHandler)
{
	std::string welcomeMsg = ":";
	welcomeMsg += clientHandler->m_user.getServername();
	welcomeMsg += " 001 ";
	welcomeMsg += clientHandler->m_user.getNickname();
	welcomeMsg += " :Welcome to the Internet Relay Network ";
	welcomeMsg += clientHandler->m_user.getNickname();
	welcomeMsg += "!";
	welcomeMsg += clientHandler->m_user.getUsername();
	welcomeMsg += "@";
	welcomeMsg += clientHandler->m_user.getHostname();
	welcomeMsg += "\r\n";

	std::cout << "Sending welcome message: " << welcomeMsg << std::endl;
	sendResponse(clientSocket, welcomeMsg);
	clientHandler->m_user.setIsRegistered(true);
}
