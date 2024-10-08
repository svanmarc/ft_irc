#include <iostream>
#include <csignal>
#include <cstdlib>
#include "Server.hpp"

// Pointeur global pour l'instance du serveur
Server *serverInstance = NULL;

// Gestionnaire de signaux
void signalHandler(int signal)
{
	if (serverInstance == NULL)
	{
		std::cerr << "Server not initialized. Exiting..." << std::endl;
		return;
	}

	if (signal == SIGINT || signal == SIGTERM)
	{
		std::cout << "\nReceived signal (" << signal << "). Stopping server..." << std::endl;
		serverInstance->stop();
		std::cout << "Server stopped. Exiting..." << std::endl;
		delete serverInstance;
		serverInstance = NULL;
		exit(0);
	}
	else
	{
		std::cerr << "Received unknown signal (" << signal << "). Ignoring..." << std::endl;
	}
}

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
		// Créer l'instance locale du serveur
		// Server server(port, serverPassword);
		// serverInstance = &server; // Associer l'instance locale au pointeur global
		serverInstance = new Server(port, serverPassword);
		std::cout << "Password set to: " << serverPassword << std::endl;

		// Configurer les signaux après la création de l'instance du serveur
		std::signal(SIGINT, signalHandler);	 // Intercepter Ctrl+C
		std::signal(SIGTERM, signalHandler); // Intercepter `kill`

		std::cout << "Server started on port " << port << ". Press Ctrl+C to stop." << std::endl;

		// Démarrer le serveur
		// server.start();
		serverInstance->start();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Server error: " << e.what() << std::endl;
		delete serverInstance;
		return 1;
	}

	return 0;
}
