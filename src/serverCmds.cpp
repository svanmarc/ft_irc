#include "server.hpp"

void nick_command(const std::string &command, int clientSocket)
{
	std::string nick = command.substr(5);
	std::cout << "Nick: " << nick << std::endl;
	const char *nickResponse = ":localhost 001 Welcome to the Internet Relay Network user!user@localhost\r\n";
	send(clientSocket, nickResponse, strlen(nickResponse), 0);
}