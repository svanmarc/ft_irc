#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdlib> // Pour size_t

#define PORT 6667		 // Le port utilisé par le serveur
#define MAX_CLIENTS 10	 // Nombre maximum de clients en attente
#define BUFFER_SIZE 1024 // Taille du buffer pour les messages

int main()
{
	int server_socket, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[BUFFER_SIZE];

	// Créer le socket serveur
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == 0)
	{
		std::cerr << "Échec de la création du socket." << std::endl;
		return -1;
	}

	// Configurer l'adresse du serveur
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; // Écouter sur toutes les interfaces réseau disponibles
	address.sin_port = htons(PORT);		  // Convertir le port en ordre réseau (big-endian)

	// Lier le socket à l'adresse et au port
	if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Erreur lors du bind." << std::endl;
		close(server_socket);
		return -1;
	}

	// Écouter les connexions entrantes
	if (listen(server_socket, MAX_CLIENTS) < 0)
	{
		std::cerr << "Erreur lors de l'écoute." << std::endl;
		close(server_socket);
		return -1;
	}

	std::cout << "Serveur Echo en écoute sur le port " << PORT << std::endl;

	// Initialiser le tableau pour poll()
	std::vector<pollfd> poll_fds;
	pollfd server_fd;
	server_fd.fd = server_socket;
	server_fd.events = POLLIN; // On attend de nouvelles connexions
	poll_fds.push_back(server_fd);

	while (true)
	{
		// Attendre les événements sur les sockets
		int activity = poll(poll_fds.data(), poll_fds.size(), -1);
		if (activity < 0)
		{
			std::cerr << "Erreur de poll()" << std::endl;
			break;
		}

		// Vérifier si une nouvelle connexion arrive
		if (poll_fds[0].revents & POLLIN)
		{
			new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
			if (new_socket < 0)
			{
				std::cerr << "Erreur d'acceptation de la connexion." << std::endl;
			}
			else
			{
				pollfd client_fd;
				client_fd.fd = new_socket;
				client_fd.events = POLLIN;
				poll_fds.push_back(client_fd);

				std::cout << "Nouveau client connecté." << std::endl;
			}
		}

		// Vérifier les événements sur les autres sockets (clients connectés)
		for (size_t i = 1; i < poll_fds.size(); ++i)
		{
			if (poll_fds[i].revents & POLLIN)
			{
				// Lire les données du client
				int valread = read(poll_fds[i].fd, buffer, BUFFER_SIZE);
				if (valread <= 0)
				{
					// Le client s'est déconnecté
					std::cout << "Client déconnecté." << std::endl;
					close(poll_fds[i].fd);
					poll_fds.erase(poll_fds.begin() + i);
					--i; // Ajuster l'index après suppression
				}
				else
				{
					buffer[valread] = '\0'; // S'assurer que le message est une chaîne valide
					std::cout << "Reçu : " << buffer << std::endl;
					send(poll_fds[i].fd, buffer, valread, 0); // Réenvoyer les données au client
				}
			}
		}
	}

	close(server_socket);
	return 0;
}
