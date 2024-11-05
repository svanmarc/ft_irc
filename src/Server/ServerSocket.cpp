#include "CommandHandler.hpp"
#include "Server.hpp"

// Initialisation du socket avec le port fourni
void Server::setupSocket(const int port) {
    m_serverSocket = createSocket(); // Créer le socket d'écoute
    if (m_serverSocket < 0)
        throw std::runtime_error("Failed to create server socket");
    if (setSocketOptions() < 0)
        throw std::runtime_error("Failed to set socket options");
    if (bindSocket(port) < 0) // Utiliser le port pour attacher le socket
        throw std::runtime_error("Failed to bind server socket to port");
    if (listen(m_serverSocket, 10) < 0) {
        std::cerr << RED << "Failed to listen on server socket" << RESET << std::endl;
        close(m_serverSocket);
        throw std::runtime_error("Failed to listen on server socket");
    }
}

// Créer le socket du serveur
int Server::createSocket() {
    const int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << RED << "Failed to create server socket: " << std::strerror(errno) << RESET << std::endl;
        return -1;
    }

    // Rendre le socket non-bloquant
    if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << RED << "Failed to set non-blocking socket: " << std::strerror(errno) << RESET << std::endl;
        close(sock);
        return -1;
    }

    return sock;
}


// Configurer les options du socket
int Server::setSocketOptions() const {
    int opt = 1;
    // Configure SO_REUSEADDR pour réutiliser l'adresse immédiatement
    if (setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << RED << "Failed to set SO_REUSEADDR: " << std::strerror(errno) << RESET << std::endl;
        return -1;
    }
    // Configurer SO_KEEPALIVE pour détecter les déconnexions
    if (setsockopt(m_serverSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0) {
        std::cerr << RED << "Failed to set SO_KEEPALIVE: " << std::strerror(errno) << RESET << std::endl;
        return -1;
    }
    return 0;
}

// Attacher le socket à l'adresse et au port
int Server::bindSocket(int port) {
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = INADDR_ANY;
    m_serverAddress.sin_port = htons(port);
    if (bind(m_serverSocket, reinterpret_cast<sockaddr *>(&m_serverAddress), sizeof(m_serverAddress)) < 0) {
        std::cerr << RED << "Failed to bind socket to port: " << std::strerror(errno) << RESET << std::endl;
        return -1;
    }
    return 0;
}