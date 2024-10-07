#include "CommandHandler.hpp"

const std::string CommandHandler::WELCOME_MESSAGE = "Welcome to the Internet Relay Network 2024 ";

void CommandHandler::handleCommand(const std::string &command, int clientSocket, ClientHandler *clientHandler, Server &server)
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
			handlePass(command, clientSocket, clientHandler, server);
		}
		else if (cmd == "USERHOST")
		{
			std::cout << "USERHOST command received" << std::endl;
			handleUser(command, clientSocket, clientHandler);
		}
		else if (cmd == "MODE")
		{
			handleMode(command, clientSocket, clientHandler);
		}
		else if (cmd == "WHOIS")
		{
			handleWhois(clientSocket, clientHandler);
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

void CommandHandler::handlePass(const std::string &command, int clientSocket, ClientHandler *clientHandler, Server &server)
{
	size_t pos = command.find("PASS ");
	if (pos != std::string::npos && pos + 5 < command.length())
	{
		std::string clientPassword = trim(command.substr(pos + 5));

		if (server.authenticate(clientPassword))
		{
			clientHandler->setAuthenticated(true);
			clientHandler->resetAttempts(); // Réinitialisez les tentatives en cas de succès
			sendResponse(clientSocket, "Authentication successful! Welcome.");
		}
		else
		{
			clientHandler->incrementAttempts(); // Incrémentez les tentatives
			if (clientHandler->getAttempts() >= 3)
			{
				sendResponse(clientSocket, "Too many failed attempts. Disconnecting...");
				close(clientSocket); // Fermez la connexion après 3 échecs
			}
			else
			{
				std::ostringstream oss; // Utilisez un ostringstream pour la conversion
				oss << (3 - clientHandler->getAttempts());
				sendResponse(clientSocket, "Incorrect password! Please try again. Attempts left: " + oss.str());
			}
		}
	}
	else
	{
		sendResponse(clientSocket, "Error: No password provided.");
		close(clientSocket);
	}
}

void CommandHandler::handleMode(const std::string &command, int clientSocket, ClientHandler *clientHandler)
{
	std::string mode;
	size_t pos = command.find("MODE");
	if (pos != std::string::npos && pos + 5 < command.length())
	{
		mode = trim(command.substr(pos + 5));
	}

	clientHandler->m_user.setUserMode(mode);

	std::string response = clientHandler->m_user.getNickname();
	response += " +";
	response += mode;
	sendResponse(clientSocket, RPL_CHANNELMODEIS, response);
}

void CommandHandler::handleWhois(int clientSocket, ClientHandler *clientHandler)
{
	std::string response = "NICK ";
	response += clientHandler->m_user.getNickname();
	sendResponse(clientSocket, RPL_WELCOME, response);
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
	sendResponse(clientSocket, 0, message);
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
