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
#include "ClientHandler.hpp"

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
	int port;
	std::string password;

	void setupSocket(int port);
	void acceptClient();
	int createSocket();
	int setSocketOptions();
	int bindSocket();
};

#endif
