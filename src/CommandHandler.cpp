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
			handleWhois(command, clientSocket, clientHandler, server);
		}
		else if (cmd == "QUIT")
		{
			handleQuit(clientSocket, clientHandler, server);
		}
		else if (cmd == "PRIVMSG")
		{
			handlePrivMsg(command, clientSocket, clientHandler, server);
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

	clientHandler->setNickname(nickname);

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
		sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "missing parameters: USER <username> <hostname> <servername> <realname>");
		return;
	}

	// Vérifier si l'utilisateur est déjà enregistré
	if (clientHandler->getUser().isRegistered())
	{
		sendResponse(clientSocket, ERR_ALREADYREGISTRED, "You are already registered");
		return;
	}

	// parts[0] est "USER", on l'ignore
	std::string username = parts[1];
	std::string hostname = parts[2];
	std::string servername = parts[3];
	std::string realname = parts[4];

	clientHandler->getUser().setUsername(username);
	clientHandler->getUser().setHostname(hostname);
	clientHandler->getUser().setRealname(realname);

	if (!clientHandler->getUser().getNickname().empty())
	{
		completeRegistration(clientSocket, clientHandler);
	}
	else
	{
		sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "missing parameters: NICK <nickname>");
	}
}

void CommandHandler::handlePass(const std::string &command, int clientSocket, ClientHandler *clientHandler, Server &server)
{
	size_t pos = command.find("PASS ");
	if (pos != std::string::npos && pos + 5 < command.length())
	{
		std::string clientPassword = trim(command.substr(pos + 5));

		std::cout << "Client password: " << clientPassword << std::endl;

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

	clientHandler->getUser().setUserMode(mode);

	std::string response = clientHandler->getNickname();
	response += " +";
	response += mode;
	sendResponse(clientSocket, RPL_CHANNELMODEIS, response);
}

// void CommandHandler::sendResponse(int clientSocket, int code, const std::string &message)
// {
// 	std::string response = ":localhost ";
// 	std::ostringstream oss;
// 	oss << code;
// 	response += oss.str();
// 	response += " " + message + "\r\n";
// 	std::cout << "Sending response: " << response << std::endl;
// 	send(clientSocket, response.c_str(), response.length(), 0);
// }

void CommandHandler::sendResponse(int clientSocket, int code, const std::string &message)
{
	std::string response = ":localhost ";
	if (code != 0) // N'ajouter le code que s'il est différent de 0
	{
		std::ostringstream oss;
		oss << code;
		response += oss.str() + " ";
	}
	response += message + "\r\n";
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
	clientHandler->getUser().setIsRegistered(true);
}

void CommandHandler::handleQuit(int clientSocket, ClientHandler *clientHandler, Server &server)
{
	sendResponse(clientSocket, "Goodbye!");
	std::cout << "Client " << clientHandler->getNickname() << " has quit." << std::endl;
	server.handleClientDisconnect(clientSocket);
}

void CommandHandler::handlePrivMsg(const std::string &command, int clientSocket, ClientHandler *clientHandler, Server &server)
{
	// Extraire le destinataire et le message
	std::string::size_type start = command.find("PRIVMSG ");
	if (start == std::string::npos)
	{
		sendResponse(clientSocket, ERR_UNKNOWNCOMMAND, "Unknown command: " + command);
		return;
	}

	// Extraire le `target`
	start += 8; // Sauter "PRIVMSG "
	std::string::size_type spacePos = command.find(' ', start);
	if (spacePos == std::string::npos)
	{
		sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "No target specified");
		return;
	}
	std::string target = command.substr(start, spacePos - start);

	// Extraire le message après le symbole ':'
	std::string::size_type messagePos = command.find(':', spacePos);
	if (messagePos == std::string::npos)
	{
		sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "No message specified");
		return;
	}
	std::string message = command.substr(messagePos + 1);

	// Envoi du message au destinataire spécifié
	bool found = false;
	const std::vector<ClientHandler *> &clients = server.getClients(); // Utiliser le getter de `Server` pour récupérer les clients

	for (std::vector<ClientHandler *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == target) // Utiliser `getNickname()` pour accéder au nom d'utilisateur
		{
			found = true;
			std::string response = ":" + clientHandler->getNickname() + " PRIVMSG " + target + " :" + message;
			(*it)->sendResponse(response); // Envoie le message au client spécifié
			break;
		}
	}

	if (!found)
	{
		std::cout << "PRIVMSG from " << clientHandler->getNickname()
				  << " to " << target << ": " << message << std::endl;
		sendResponse(clientSocket, "No such user: " + target);
	}
}

void CommandHandler::handleWhois(const std::string &command, int clientSocket, ClientHandler *clientHandler, Server &server)
{
	std::string::size_type start = command.find("WHOIS ");
	if (start == std::string::npos)
	{
		sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "WHOIS :No nickname given");
		return;
	}

	// Extraire le `nickname` cible
	start += 6;												  // Sauter "WHOIS "
	std::string targetNickname = trim(command.substr(start)); // Utiliser `trim` pour supprimer les espaces autour du pseudo

	// Vérifier si le `nickname` est vide après extraction
	if (targetNickname.empty())
	{
		sendResponse(clientSocket, ERR_NEEDMOREPARAMS, "WHOIS :No nickname given");
		return;
	}

	// Chercher le client correspondant au `targetNickname`
	const std::vector<ClientHandler *> &clients = server.getClients(); // Récupérer les clients
	bool found = false;

	for (std::vector<ClientHandler *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
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
			return;
		}
	}

	if (!found)
	{
		sendResponse(clientSocket, "No such nickname: " + targetNickname);
	}
}
