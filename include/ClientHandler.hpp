#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cerrno>
#include "User.hpp"

class Server;

class ClientHandler
{
public:
	ClientHandler(int socket);
	~ClientHandler();

	void handlerClient(Server &server);

	// Gestion de l'authentification et des tentatives
	bool isAuthenticated() const;
	void setAuthenticated(bool isAuthenticated);
	int getAttempts() const;
	void incrementAttempts();
	void resetAttempts();

	// Gestion des informations de l'utilisateur
	User &getUser();
	const std::string &getNickname() const;
	void setNickname(const std::string &nickname);

	// Communication avec le client
	int getSocket() const;
	void sendResponse(const std::string &response);

	// Gestion des canaux
	void joinChannel(const std::string &channel);
	void leaveChannel(const std::string &channel);

private:
	int clientSocket;
	int attempts;
	bool authenticated;
	User m_user;
	std::vector<std::string> channels;

	void readCommand(const std::string &command, Server &server);
};

#endif
