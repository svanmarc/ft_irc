#include "Server.hpp"
#include "CommandHandler.hpp"

Server::Server(int port, const std::string &password) : password(password)
{
	setupSocket(port);
	commandHandler = new CommandHandler(*this);

	// Ajouter le socket d'écoute à la liste des descripteurs surveillés par `poll`
	struct pollfd serverPollFD;
	serverPollFD.fd = serverSocket; // Socket du serveur
	serverPollFD.events = POLLIN;	// Surveiller les événements de lecture
	serverPollFD.revents = 0;		// Initialisation de l'état
	fds.push_back(serverPollFD);	// Ajouter au vecteur `fds`
}

void Server::start()
{
	std::cout << "Server is running. Waiting for connections..." << std::endl;

	while (true)
	{
		// Appeler `poll` pour surveiller les sockets de tous les clients
		int pollCount = poll(&fds[0], fds.size(), -1);
		if (pollCount < 0)
		{
			std::cerr << "Error in poll: " << std::strerror(errno) << std::endl;
			break;
		}

		for (unsigned int i = 0; i < fds.size(); ++i)
		{
			// Si le socket surveillé est le socket d'écoute, accepter une nouvelle connexion
			if (fds[i].fd == serverSocket && fds[i].revents & POLLIN)
			{
				acceptClient(); // Accepter la nouvelle connexion
			}
			// Si un client envoie un message
			else if (fds[i].revents & POLLIN)
			{
				// Appelle `handlerClient` pour chaque client mais sans bloquer le reste du serveur
				ClientHandler *client = findClient(fds[i].fd);
				if (client)
					client->handlerClient(*this); // Traitement d'un seul message à la fois
			}
		}
	}
}

Server::~Server()
{
	stop();
	if (commandHandler != 0)
	{
		delete commandHandler;
		commandHandler = 0;
	}
}

void Server::stop()
{
	std::cout << "Stopping server..." << std::endl;

	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i] != 0)
		{
			delete clients[i];
			clients[i] = 0;
		}
	}
	clients.clear();

	std::vector<ClientHandler *>().swap(clients);

	fds.clear();

	std::vector<pollfd>().swap(fds);

	if (serverSocket >= 0)
	{
		close(serverSocket);
		serverSocket = -1; // Mettre à jour pour indiquer qu'il est fermé
	}

	std::cout << "Server stopped --- Welcome to the real world." << std::endl;
}

// Initialisation du socket avec le port fourni
void Server::setupSocket(int port)
{
	serverSocket = createSocket(); // Créer le socket d'écoute
	if (serverSocket < 0)
		throw std::runtime_error("Failed to create server socket");

	if (setSocketOptions() < 0)
		throw std::runtime_error("Failed to set socket options");

	if (bindSocket(port) < 0) // Utiliser le port pour attacher le socket
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
int Server::bindSocket(int port)
{
	serverAddress.sin_family = AF_INET;			// Protocole IPv4
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Écoute sur toutes les interfaces
	serverAddress.sin_port = htons(port);		// Convertir le port en format réseau

	if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "Failed to bind socket to port: " << std::strerror(errno) << std::endl;
		return -1;
	}

	return 0;
}

// Accepter un nouveau client
void Server::acceptClient()
{
	int clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket < 0)
	{
		std::cerr << "Failed to accept client connection: " << std::strerror(errno) << std::endl;
		return;
	}

	std::cout << "New client connected." << std::endl;

	// Ajouter le nouveau client au vecteur `fds` pour que `poll` le surveille
	struct pollfd newClientPollFD;
	newClientPollFD.fd = clientSocket;
	newClientPollFD.events = POLLIN;
	newClientPollFD.revents = 0;
	fds.push_back(newClientPollFD);

	// Ajouter le client à la liste des clients avec un `ClientHandler`
	ClientHandler *newClient = new ClientHandler(clientSocket);
	clients.push_back(newClient);
}

// Gérer les messages d'un client existant
void Server::handleClient(int clientSocket)
{
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
	if (bytesRead <= 0)
	{
		std::cerr << "Client disconnected or error reading: " << std::strerror(errno) << std::endl;
		close(clientSocket);

		for (unsigned int i = 0; i < fds.size(); ++i)
		{
			if (fds[i].fd == clientSocket)
			{
				fds.erase(fds.begin() + i);
				break;
			}
		}

		for (unsigned int i = 0; i < clients.size(); ++i)
		{
			if (clients[i]->getSocket() == clientSocket)
			{
				delete clients[i];
				clients.erase(clients.begin() + i);
				break;
			}
		}
		return;
	}

	std::string message(buffer);
	std::cout << "Client command: " << message << std::endl;

	for (unsigned int i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getSocket() == clientSocket)
		{
			commandHandler->handleCommand(message, clientSocket, clients[i], *this);
			break;
		}
	}
}

// Retirer un client du vecteur `clients`
void Server::removeClient(int clientSocket)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getSocket() == clientSocket)
		{
			delete clients[i]; // Libérer la mémoire allouée pour le client
			clients.erase(clients.begin() + i);
			break;
		}
	}

	// Retirer également le client de `fds`
	for (size_t i = 0; i < fds.size(); ++i)
	{
		if (fds[i].fd == clientSocket)
		{
			fds.erase(fds.begin() + i);
			break;
		}
	}
}

// Trouver un client par son socket
ClientHandler *Server::findClient(int clientSocket)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getSocket() == clientSocket)
			return clients[i];
	}
	return NULL;
}

// Envoyer un message de réponse au client
void Server::sendResponse(int clientSocket, const std::string &message)
{
	std::string response = ":localhost " + message + "\r\n";
	send(clientSocket, response.c_str(), response.length(), 0);
}

// Vérifier le mot de passe du client
bool Server::checkPassword(const std::string &clientPassword)
{
	return clientPassword == password;
}

bool Server::authenticate(const std::string &clientPassword)
{
	return checkPassword(clientPassword);
}

void Server::handleClientDisconnect(int clientSocket)
{
	std::cout << "Handing disconnection for client socket: " << std::endl;
	removeClient(clientSocket);
}
