#pragma once
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <ctime>

class ClientHandler
{
	public:
		ClientHandler(int socket);
		~ClientHandler();
		
		void setNickname(const std::string& nickname);
		const std::string& getNickname() const;

		void setUserMode(const std::string& userNode);
		const std::string& getUserMode() const;

		void joinChannel(const std::string& channel);
		void leaveChannel(const std::string& channel);
		void handlerClient();
	
	private:
		int clientSocket;
		std::string nickname;
		std::string userMode;
		std::vector<std::string> channels;
		bool isConnected;
		std::time_t connectionTime;
		std::time_t lastActivityTime;
		void readCommand(const std::string &command);
		void sendResponse(const std::string& response);

};