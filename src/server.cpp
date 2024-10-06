#include "server.hpp"

Server::Server(int port, const std::string &password) : port(port), password(password)
{
	setupSocket(port);
}

Server::~Server()
{
	stop();
}

void Server::start()
{
	while (true)
	{
		acceptClient();
	}
}

void Server::stop()
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		delete clients[i];
	}
	close(serverSocket);
	std::cout << "Server stopped --- Welcome to the real world " << std::endl;
}

// Configuration du socket du serveur
void Server::setupSocket(int port)
{
	serverSocket = createSocket();
	if (serverSocket < 0)
		throw std::runtime_error("Failed to create server socket");

	if (setSocketOptions() < 0)
		throw std::runtime_error("Failed to set socket options");

	if (bindSocket() < 0)
		throw std::runtime_error("Failed to bind server socket to port");

	if (listen(serverSocket, 10) < 0)
	{
		std::cerr << "Failed to listen on server socket" << std::endl;
		close(serverSocket);
		throw std::runtime_error("Failed to listen on server socket");
	}

	std::cout << "Server started on port " << port << std::endl;
}

// Créer le socket du serveur
int Server::createSocket()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		std::cerr << "Failed to create server socket: " << std::strerror(errno) << std::endl;
		return -1;
	}
	return sock;
}

// Configurer les options du socket
int Server::setSocketOptions()
{
	int opt = 1;

	// Configurer SO_REUSEADDR pour réutiliser l'adresse immédiatement
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cerr << "Failed to set SO_REUSEADDR: " << std::strerror(errno) << std::endl;
		return -1;
	}

	// Configurer SO_KEEPALIVE pour détecter les déconnexions
	if (setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0)
	{
		std::cerr << "Failed to set SO_KEEPALIVE: " << std::strerror(errno) << std::endl;
		return -1;
	}

	return 0;
}

// Attacher le socket à l'adresse et au port
int Server::bindSocket()
{
	serverAddress.sin_family = AF_INET;			// Protocole IPv4
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Ecoute sur toutes les interfaces
	serverAddress.sin_port = htons(port);		// Convertir le port en format réseau

	// Appeler bind() pour attacher le socket
	if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "Failed to bind socket to port: " << std::strerror(errno) << std::endl;
		return -1;
	}

	return 0;
}

// Accepter les connexions entrantes et gérer les commandes
void Server::acceptClient()
{
	int clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket < 0)
	{
		std::cerr << "Failed to accept client connection: " << std::strerror(errno) << std::endl;
		return;
	}

	std::cout << "New client connected. Awaiting password verification..." << std::endl;

	// Lire la commande envoyée par le client
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
	if (bytesRead <= 0)
	{
		std::cerr << "Failed to read from client: " << std::strerror(errno) << std::endl;
		close(clientSocket);
		return;
	}

	// Extraire la commande envoyée par le client
	std::string command(buffer);
	command.erase(std::remove(command.begin(), command.end(), '\n'), command.end());
	command.erase(std::remove(command.begin(), command.end(), '\r'), command.end());

	// Vérifier si la commande est une commande `PASS`
	if (command.find("PASS") == 0)
	{
		// Extraire le mot de passe après "PASS "
		std::string clientPassword = command.substr(5);

		std::cout << "Client sent password: [" << clientPassword << "]" << std::endl;

		// Comparer avec le mot de passe du serveur
		if (clientPassword != password)
		{
			std::cerr << "Incorrect password from client. Closing connection." << std::endl;
			close(clientSocket);
			return;
		}

		std::cout << "Client authenticated successfully!" << std::endl;

		// Stocker le client et gérer la connexion
		ClientHandler *newClient = new ClientHandler(clientSocket);
		clients.push_back(newClient);

		// Gérer le client (pour l'instant, fermer immédiatement)
		newClient->handlerClient();
	}
	else
	{
		std::cerr << "Invalid command. Expected PASS. Closing connection." << std::endl;
		close(clientSocket);
	}
}
