#include "server.hpp"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int port = std::atoi(argv[1]);
	if (port <= 0 || port > 65535)
	{
		std::cerr << "Invalid port number. Must be between 1 and 65535." << std::endl;
		return 1;
	}

	std::string serverPassword = argv[2];

	try
	{
		Server server(port, serverPassword);
		std::cout << "Password set to: " << serverPassword << std::endl;

		server.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Server error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
