#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

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
	ClientHandler(int socket);
	~ClientHandler();

	void joinChannel(const std::string &channel);
	void leaveChannel(const std::string &channel);
	void handlerClient(Server &server);
	int getSocket() const;

	bool isAuthenticated() const;
	void setAuthenticated(bool isAuthenticated);

	int getAttempts() const;
	void incrementAttempts();
	void resetAttempts();

	User m_user;

private:
	int clientSocket;
	std::vector<std::string> channels;
	int attempts;
	void readCommand(const std::string &command, Server &server);
	void sendResponse(const std::string &response);
};

#endif
