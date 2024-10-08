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
#include "Channel.hpp"
class Server
{
	public:
		Server(int port);
		~Server();
		void start();
		void stop();
		void joinChannel(const User& newUser,const std::string& name);
		bool checkIfChannelExists(const std::vector<Channel> channels, const std::string& name);
		Channel getChannel(const std::string& name);

	private:
		int m_serverSocket;
		int m_port;
		sockaddr_in m_serverAddress;
		std::vector<ClientHandler*> m_clients;
		std::vector<Channel> m_channels;

		void setupSocket(int port);
		void acceptClient();
};
