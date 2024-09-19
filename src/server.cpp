#include "server.hpp"
#include <sstream>

void handle_command(const std::string &command, int clientSocket)
{
	if (command.find("NICK") == 0)
	{
		const char *nickResponse = ":localhost 001 Welcome to the Internet Relay Network user!user@localhost\r\n";
		send(clientSocket, nickResponse, strlen(nickResponse), 0);
		std::cout << "Handled NICK command" << std::endl;
	}
	else
	{
		const char *unknownResponse = ":localhost 421 Unknown command\r\n";
		send(clientSocket, unknownResponse, strlen(unknownResponse), 0);
	}
}

void start_server(int port)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == 0)
	{
		std::cerr << "Failed to create a socket" << std::endl;
		return;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "Failed to bind the socket" << std::endl;
		return;
	}

	if (listen(serverSocket, 3) < 0)
	{
		std::cerr << "Failed to listen for incoming connections" << std::endl;
		return;
	}

	std::cout << "Server is running on port " << port << std::endl;

	while (true)
	{
		int clientSocket = accept(serverSocket, NULL, NULL);
		if (clientSocket < 0)
		{
			std::cerr << "Failed to accept a connection" << std::endl;
			return;
		}

		while (true)
		{
			char buffer[1024] = {0};
			int bytesRead = read(clientSocket, buffer, sizeof(buffer));
			if (bytesRead <= 0)
			{
				std::cerr << "Client disconnected or error reading" << std::endl;
				break;
			}

			// Convertir le buffer en un flux de chaînes
			std::istringstream stream(buffer);
			std::string command;

			// Traiter chaque commande séparément
			while (std::getline(stream, command))
			{
				if (!command.empty())
				{
					std::cout << "Received command: " << command << std::endl;
					handle_command(command, clientSocket);
				}
			}
		}

		close(clientSocket);
	}

	close(serverSocket);
}

// void start_server(int port)
// {
// 	// Create a socket
// 	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	if (serverSocket == 0)
// 	{
// 		std::cerr << "Failed to create a socket" << std::endl;
// 		return;
// 	}

// 	// Define the server address
// 	sockaddr_in serverAddress;
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_port = htons(port);
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;

// 	// Bind the socket to the server address
// 	if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
// 	{
// 		std::cerr << "Failed to bind the socket" << std::endl;
// 		return;
// 	}

// 	// Listen for incoming connections
// 	if (listen(serverSocket, 3) < 0)
// 	{
// 		std::cerr << "Failed to listen for incoming connections" << std::endl;
// 		return;
// 	}

// 	// Accept incoming connections
// 	while (true)
// 	{
// 		// Accept a connection
// 		int clientSocket = accept(serverSocket, NULL, NULL);
// 		if (clientSocket < 0)
// 		{
// 			std::cerr << "Failed to accept a connection" << std::endl;
// 			return;
// 		}

// 		// Loop for managing commands from client
// 		while (true)
// 		{
// 			// Receive a message from the client
// 			char buffer[1024] = {0};
// 			int bytesRead = read(clientSocket, buffer, sizeof(buffer));
// 			if (bytesRead <= 0)
// 			{
// 				std::cerr << "Client disconnected or error reading" << std::endl;
// 				break;
// 			}

// 			std::cout << "Received: " << buffer << std::endl;

// 			// Handle the message
// 			if (strncmp(buffer, "CAP LS", 6) == 0)
// 			{
// 				const char *capResponse = ":localhost CAP * LS :account-notify extended-join multi-prefix\r\n";
// 				send(clientSocket, capResponse, strlen(capResponse), 0);
// 			}
// 			else if (strncmp(buffer, "CAP REQ", 7) == 0)
// 			{
// 				const char *capResponse = ":localhost CAP * ACK :account-notify extended-join multi-prefix\r\n";
// 				send(clientSocket, capResponse, strlen(capResponse), 0);
// 			}
// 			else if (strncmp(buffer, "CAP END", 7) == 0)
// 			{
// 				const char *capResponse = ":localhost CAP * ACK :account-notify extended-join multi-prefix\r\n";
// 				send(clientSocket, capResponse, strlen(capResponse), 0);
// 			}
// 			else if (strncmp(buffer, "NICK", 4) == 0)
// 			{
// 				const char *nickResponse = ":localhost 001 Welcome to the Internet Relay Network user!user@localhost\r\n";
// 				send(clientSocket, nickResponse, strlen(nickResponse), 0);
// 			}
// 			else if (strncmp(buffer, "USER", 4) == 0)
// 			{
// 				const char *userResponse = ":localhost 002 Welcome to the Internet Relay Network user!user@localhost\r\n";
// 				send(clientSocket, userResponse, strlen(userResponse), 0);
// 			}
// 			else if (strncmp(buffer, "PING", 4) == 0)
// 			{
// 				const char *pongResponse = "PONG localhost\r\n";
// 				send(clientSocket, pongResponse, strlen(pongResponse), 0);
// 			}
// 			else if (strncmp(buffer, "MODE", 4) == 0)
// 			{
// 				const char *modeResponse = ":localhost 324 Welcome +i\r\n";
// 				send(clientSocket, modeResponse, strlen(modeResponse), 0);
// 			}
// 			else if (strncmp(buffer, "WHOIS", 5) == 0)
// 			{
// 				const char *whoisResponse = ":localhost 311 user user user localhost * :real name\r\n";
// 				send(clientSocket, whoisResponse, strlen(whoisResponse), 0);
// 			}
// 			else if (strncmp(buffer, "AUTHENTICATE", 12) == 0)
// 			{
// 				// Ignore SASL for now, but respond with an error if needed
// 				const char *authResponse = ":localhost 904 Authentication failed\r\n";
// 				send(clientSocket, authResponse, strlen(authResponse), 0);
// 			}
// 			else if (strncmp(buffer, "QUIT", 4) == 0)
// 			{
// 				const char *quitResponse = ":localhost 001 Goodbye\r\n";
// 				send(clientSocket, quitResponse, strlen(quitResponse), 0);
// 				break;
// 			}
// 			else
// 			{
// 				const char *unknownResponse = ":localhost 421 Unknown command\r\n";
// 				send(clientSocket, unknownResponse, strlen(unknownResponse), 0);
// 			}

// 			// Clear the buffer
// 			memset(buffer, 0, sizeof(buffer));
// 		}
// 		// Close the client socket
// 		close(clientSocket);
// 	}
// 	// Close the server socket
// 	close(serverSocket);
// }
