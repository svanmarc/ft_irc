#include "ClientHandler.hpp"
#include "CommandHandler.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>

ClientHandler::ClientHandler(int socket) : clientSocket(socket) {}

ClientHandler::~ClientHandler()
{
    close(clientSocket);
}

void ClientHandler::handlerClient()
{
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            std::cerr << "Client disconnected or error reading" << std::endl;
            break;
        }
        std::string command(buffer);
        std::cout << "Received command: " << command << std::endl;
        readCommand(command);
    }
}

void ClientHandler::readCommand(const std::string &command)
{
	CommandHandler commandHandler;
	commandHandler.handleCommand(command, clientSocket, this);
}

void ClientHandler::sendResponse(const std::string& response)
{
    send(clientSocket, response.c_str(), response.length(), 0);
}
void ClientHandler::setNickname(const std::string& nickname)
{
	this->nickname = nickname;
}
void ClientHandler::setUserMode(const std::string& userNode)
{
	this->userMode = userNode;
}

const std::string& ClientHandler::getNickname() const
{
	return nickname;
}

const std::string& ClientHandler::getUserMode() const
{
	return userMode;
}

void ClientHandler::joinChannel(const std::string& channel)
{
	channels.push_back(channel);
}

void ClientHandler::leaveChannel(const std::string& channel)
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
