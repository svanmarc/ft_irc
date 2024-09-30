#include "server.hpp"

Server::Server(int port)
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

void Server::setupSocket(int port)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 10);

    std::cout << "Server started on port " << port << std::endl;
}

void Server::acceptClient()
{
    int clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket >= 0)
    {
        std::cout << "New client connected" << std::endl;
        ClientHandler* newClient = new ClientHandler(clientSocket);  // Utilisation des pointeurs bruts
        clients.push_back(newClient);
        newClient->handlerClient();
    }
}