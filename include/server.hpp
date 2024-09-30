#pragma once
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <vector>
#include "ClientHandler.hpp"

class Server
{
	public:
		Server(int port);
		~Server();
		void start();
		void stop();

	private:
		int serverSocket;
		sockaddr_in serverAddress;
		std::vector<ClientHandler*> clients;
		int port;

		void setupSocket(int port);
		void acceptClient();
};
