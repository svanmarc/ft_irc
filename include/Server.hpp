#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include "ClientHandler.hpp"

class CommandHandler;

class Server
{
public:
	Server(int port, const std::string &password);
	~Server();

	void start();
	void stop();
	bool authenticate(const std::string &clientPassword);
	void handleClientDisconnect(int clientSocket);

	// Accès aux clients connectés
	const std::vector<ClientHandler *> &getClients() const { return clients; }

private:
	int serverSocket;
	sockaddr_in serverAddress;
	std::string password;
	std::vector<ClientHandler *> clients;
	std::vector<struct pollfd> fds;
	CommandHandler *commandHandler;

	// Méthodes internes
	void setupSocket(int port);
	void acceptClient();
	void handleClient(int clientSocket);
	int createSocket();
	int setSocketOptions();
	int bindSocket(int port);
	bool checkPassword(const std::string &clientPassword);
	void removeClient(int clientSocket);
	ClientHandler *findClient(int clientSocket);
	void sendResponse(int clientSocket, const std::string &message);
};

#endif
