#include "server.hpp"

void handle_command(const std::string &command, int clientSocket)
{
	if (command.find("NICK") == 0)
	{
		// const char *nickResponse = ":localhost 001 Welcome to the Internet Relay Network user!user@localhost\r\n";
		// send(clientSocket, nickResponse, strlen(nickResponse), 0);
		// std::cout << "Handled NICK command" << std::endl;
		nick_command(command, clientSocket);
	}
	else
	{
		const char *unknownResponse = ":localhost 421 Unknown command\r\n";
		send(clientSocket, unknownResponse, strlen(unknownResponse), 0);
	}
}

void handle_client(int clientSocket)
{
	char buffer[1024] = {0};

	while (true)
	{
		int bytesRead = read(clientSocket, buffer, sizeof(buffer));
		if (bytesRead <= 0)
		{
			std::cerr << "Client disconnected or error reading" << std::endl;
			break;
		}

		std::istringstream stream(buffer);
		std::string command;
		while (std::getline(stream, command))
		{
			if (!command.empty())
			{
				std::cout << "Received command: " << command << std::endl;
				handle_command(command, clientSocket);
			}
		}
		memset(buffer, 0, sizeof(buffer));
	}
	close(clientSocket);
}

int setup_server_socket(int port)
{
	// Create a socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		return error_setup_socket_serv("Failed to create a socket");

	// Define the server address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket to the server address
	if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		return error_setup_socket_serv("Failed to bind the socket");

	// Listen for incoming connections
	if (listen(serverSocket, 3) < 0)
		return error_setup_socket_serv("Failed to listen for incoming connections");

	std::cout << "Server is running on port " << port << std::endl;

	return serverSocket;
}

void start_server(int port)
{
	// Create a socket
	int serverSocket = setup_server_socket(port);
	if (serverSocket < 0)
		return;

	// Accept incoming connections
	while (true)
	{
		int clientSocket = accept(serverSocket, NULL, NULL);
		if (clientSocket < 0)
		{
			std::cerr << "Failed to accept a connection" << std::endl;
			return;
		}

		std::cout << "Client connected" << std::endl;
		handle_client(clientSocket);
	}
	close(serverSocket);
}
