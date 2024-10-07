#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <cerrno>
#include <algorithm>
#include <poll.h>
#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

class Server
{
public:
	Server(int port, const std::string &password);
	~Server();
	void start();
	void stop();

private:
	int serverSocket;
	sockaddr_in serverAddress;
	std::vector<ClientHandler *> clients;
	std::string password;
	std::vector<struct pollfd> fds;
	CommandHandler commandHandler;

	void setupSocket(int port);
	void acceptClient();
	void handleClient(int clientSocket);
	int createSocket();
	int setSocketOptions();
	int bindSocket(int port);

	void removeClient(int clientSocket);
	ClientHandler *findClient(int clientSocket);
	void sendResponse(int clientSocket, const std::string &message);
};

#endif
