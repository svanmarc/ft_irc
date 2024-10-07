#pragma once
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <ctime>
#include "User.hpp"

class Server;

class ClientHandler
{
	public:
		ClientHandler(int socket, Server* serverRef);
		~ClientHandler();

		void joinChannel(const std::string& channel);
		void leaveChannel(const std::string& channel);
		void handlerClient();
		User user;
	
	private:
		int m_clientSocket;
		Server* m_server;
		std::vector<std::string> channels;
		void readCommand(const std::string &command);
		void sendResponse(const std::string& response);
};