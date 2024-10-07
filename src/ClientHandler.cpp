#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

ClientHandler::ClientHandler(int socket) : clientSocket(socket), attempts(0) {}

ClientHandler::~ClientHandler()
{
	close(clientSocket);
}

void ClientHandler::handlerClient(Server &server)
{
	char buffer[1024];
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
		if (bytesRead <= 0)
		{
			std::cerr << "Client disconnected or error reading" << std::endl;
			break;
		}
		std::string command(buffer);
		std::cout << "Received command: " << command << std::endl;
		readCommand(command, server);
	}
}

void ClientHandler::readCommand(const std::string &command, Server &server)
{
	CommandHandler commandHandler(server);
	commandHandler.handleCommand(command, clientSocket, this, server);
}

void ClientHandler::sendResponse(const std::string &response)
{
	send(clientSocket, response.c_str(), response.length(), 0);
}

void ClientHandler::joinChannel(const std::string &channel)
{
	channels.push_back(channel);
}

void ClientHandler::leaveChannel(const std::string &channel)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i] == channel)
		{
			channels.erase(channels.begin() + i);
			break;
		}
	}
}

int ClientHandler::getSocket() const
{
	return clientSocket;
}

bool ClientHandler::isAuthenticated() const
{
	return m_user.isAuthenticated();
}

void ClientHandler::setAuthenticated(bool isAuthenticated)
{
	m_user.setAuthenticated(isAuthenticated);
}

int ClientHandler::getAttempts() const
{
	return attempts;
}

void ClientHandler::incrementAttempts()
{
	attempts++;
}

void ClientHandler::resetAttempts()
{
	attempts = 0;
}