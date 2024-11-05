#include "CommandHandler.hpp"
#include "Server.hpp"

// Accepter un nouveau client
void Server::acceptClient() {
    int const clientSocket = accept(m_serverSocket, NULL, NULL);
    if (clientSocket < 0) {
        std::cerr << RED << "😨 Failed to accept client connection: " << std::strerror(errno) << RESET << std::endl;
        return;
    }

    // Ajouter le nouveau client au vecteur `fds` pour que `poll` le surveille
    struct pollfd const newClientPollFD = {clientSocket, POLLIN, 0};
    fds.push_back(newClientPollFD);
    // Ajouter le client à la liste des clients avec un `ClientHandler`
    ClientHandler *newClient = new ClientHandler(clientSocket, this);
    clients.push_back(newClient);
    std::cout << YELLOW << "🚀 New client " << newClient << " connected with socket " << clientSocket << RESET
              << std::endl;
}
// Gérer les messages d'un client existant
void Server::handleClient(const int clientSocket) {
    char buffer[1024] = {0};
    ssize_t const bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) {
        std::cerr << RED << "Client disconnected or error reading: " << std::strerror(errno) << RESET << std::endl;
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
}

// Retirer un client du vecteur `clients`
void Server::removeClient(const int clientSocket) {
    std::cout << YELLOW << "🚚 Removing client with socket " << clientSocket << " from server" << RESET << std::endl;
    // Trouver et supprimer le client
    for (std::vector<ClientHandler *>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getSocket() == clientSocket) {
            close(clientSocket);
            delete *it;
            clients.erase(it);
            break;
        }
    }
    // Retirer également le client de `fds`
    for (std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); ++it) {
        if (it->fd == clientSocket) {
            fds.erase(it);
            break;
        }
    }
}


ClientHandler *Server::findClient(const int clientSocket) const {
    for (std::vector<ClientHandler *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getSocket() == clientSocket)
            return *it;
    }
    return NULL;
}

ClientHandler *Server::findClientByNickname(const std::string &nickname) const {
    for (std::vector<ClientHandler *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getNickname() == nickname) {
            return *it;
        }
    }
    return NULL;
}


// Vérifier le mot de passe du client
bool Server::checkPassword(const std::string &clientPassword) const { return clientPassword == m_password; }

// Authentifier le client
bool Server::authenticate(const std::string &clientPassword) const { return checkPassword(clientPassword); }

void Server::handleClientDisconnect(int clientSocket) {
    ClientHandler *client = findClient(clientSocket);
    if (client != NULL) {
        std::cout << YELLOW << "️⚠️ Client " << client->getNickname() << " disconnected" << RESET << std::endl;
        // Retirer le client de tous les canaux
        for (std::vector<Channel>::iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
            it->removeClient(client);
        }
        // Supprimer le client du serveur
        removeClient(clientSocket);
    }
}
