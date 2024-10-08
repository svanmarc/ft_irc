#include "Server.hpp"
#include "CommandHandler.hpp"

Server::Server(const int port, const std::string &password) : m_password(password) {
    setupSocket(port);
    commandHandler = new CommandHandler(*this);
    // Ajouter le socket d'écoute à la liste des descripteurs surveillés par `poll`
    const struct pollfd serverPollFD = {m_serverSocket, POLLIN, 0};
    // Ajouter au vecteur `fds`
    fds.push_back(serverPollFD);
}
Server::~Server() {
    stop();
    if (commandHandler != 0) {
        delete commandHandler;
        commandHandler = 0;
    }
}

void Server::start() {
    std::cout << "Server is running. Waiting for connections..." << std::endl;
    while (true) {
        // Appeler `poll` pour surveiller les sockets de tous les clients
        int const pollCount = poll(&fds[0], fds.size(), -1);
        if (pollCount < 0) {
            std::cerr << "Error in poll: " << std::strerror(errno) << std::endl;
            break;
        }
        for (unsigned int i = 0; i < fds.size(); ++i) {
            // Si le socket surveillé est le socket d'écoute, accepter une nouvelle connexion
            if (fds[i].fd == m_serverSocket && fds[i].revents & POLLIN) {
                acceptClient(); // Accepter la nouvelle connexion
            }
            // Si un client envoie un message
            else if (fds[i].revents & POLLIN) {
                // Appelle `handlerClient` pour chaque client mais sans bloquer le reste du serveur
                ClientHandler *client = findClient(fds[i].fd);
                if (client)
                    client->handlerClient(*this); // Traitement d'un seul message à la fois
            }
        }
    }
}

void Server::stop() {
    std::cout << "Stopping server..." << std::endl;

    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i] != 0) {
            delete clients[i];
            clients[i] = 0;
        }
    }
    clients.clear();
    std::vector<ClientHandler *>().swap(clients);
    fds.clear();
    std::vector<pollfd>().swap(fds);
    if (m_serverSocket >= 0) {
        close(m_serverSocket);
        m_serverSocket = -1;
    }
    std::cout << "Server stopped --- Welcome to the real world." << std::endl;
}
Channel Server::getChannel(const std::string &name) {
    for (size_t i = 0; i < m_channels.size(); i++) {
        if (m_channels[i].getName() == name) {
            return m_channels[i];
        }
    }
    throw std::runtime_error("Channel not found");
}

bool Server::checkIfChannelExists(const std::string &name) const {
    for (size_t i = 0; i < m_channels.size(); i++) {
        if (m_channels[i].getName() == name) {
            return true;
        }
    }
    return false;
}

bool Server::joinChannel(const User &newUser, const std::string &name) {
    try {
        if (checkIfChannelExists(name)) {
            std::cout << "Channel already exists add user in" << std::endl;
            getChannel(name).addUser(newUser);
            return true;
        }
        Channel newChannel(name, newUser);
        m_channels.push_back(newChannel);
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}