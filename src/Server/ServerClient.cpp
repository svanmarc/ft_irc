#include "Server.hpp"
#include "CommandHandler.hpp"

// Accepter un nouveau client
void Server::acceptClient() {
    int const clientSocket = accept(m_serverSocket, NULL, NULL);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept client connection: " << std::strerror(errno) << std::endl;
        return;
    }
    std::cout << "New client connected." << std::endl;
    // Ajouter le nouveau client au vecteur `fds` pour que `poll` le surveille
    struct pollfd const newClientPollFD = {clientSocket, POLLIN, 0};
    fds.push_back(newClientPollFD);
    // Ajouter le client à la liste des clients avec un `ClientHandler`
    ClientHandler *newClient = new ClientHandler(clientSocket, this);
    clients.push_back(newClient);
}
// Gérer les messages d'un client existant
void Server::handleClient(const int clientSocket) {
    char buffer[1024] = {0};
    ssize_t const bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) {
        std::cerr << "Client disconnected or error reading: " << std::strerror(errno) << std::endl;
        close(clientSocket);
        for (unsigned int i = 0; i < fds.size(); ++i) {
            if (fds[i].fd == clientSocket) {
                fds.erase(fds.begin() + i);
                break;
            }
        }
        for (unsigned int i = 0; i < clients.size(); ++i) {
            if (clients[i]->getSocket() == clientSocket) {
                delete clients[i];
                clients.erase(clients.begin() + i);
                break;
            }
        }
        return;
    }
    std::string message(buffer);
    std::cout << "Client command: " << message << std::endl;
    for (unsigned int i = 0; i < clients.size(); ++i) {
        if (clients[i]->getSocket() == clientSocket) {
            commandHandler->handleCommand(message, clientSocket, clients[i]);
            break;
        }
    }
}

// Retirer un client du vecteur `clients`
void Server::removeClient(int clientSocket) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getSocket() == clientSocket) {
            delete clients[i]; // Libérer la mémoire allouée pour le client
            clients.erase(clients.begin() + i);
            break;
        }
    }
    // Retirer également le client de `fds`
    for (size_t i = 0; i < fds.size(); ++i) {
        if (fds[i].fd == clientSocket) {
            fds.erase(fds.begin() + i);
            break;
        }
    }
}
// Trouver un client par son socket
ClientHandler *Server::findClient(const int clientSocket) const {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getSocket() == clientSocket)
            return clients[i];
    }
    return NULL;
}
// Vérifier le mot de passe du client
bool Server::checkPassword(const std::string &clientPassword) { return clientPassword == m_password; }
// Authentifier le client
bool Server::authenticate(const std::string &clientPassword) { return checkPassword(clientPassword); }

void Server::handleClientDisconnect(int clientSocket) {
    std::cout << "Handing disconnection for client socket: " << std::endl;
    removeClient(clientSocket);
}