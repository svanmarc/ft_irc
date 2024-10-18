#include "Server.hpp"
#include "CommandHandler.hpp"

Server::Server(const int port, const std::string &password) : m_password(password) {
    m_serverName = "irc.svanmarc_mrabat";
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
    for (std::vector<ClientHandler *>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (*it != 0) {
            delete *it;
            *it = 0;
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

Channel &Server::getChannel(std::string &name) {
    for (std::vector<Channel>::iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
        if (it->getName() == name) {
            return *it;
        }
    }
    throw std::runtime_error("Channel not found");
}

bool Server::checkIfChannelExists(const std::string &name) const {
    for (std::vector<Channel>::const_iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
        if (it->getName() == name) {
            return true;
        }
    }
    return false;
}

// bool Server::joinChannel(ClientHandler *newClient, std::string &name) {
//     try {
//         if (checkIfChannelExists(name)) {
//             std::cout << "Channel already exists add user in" << std::endl;
//             getChannel(name).addClient(newClient);
//             return true;
//         }
//         Channel const newChannel(name, newClient);
//         m_channels.push_back(newChannel);
//         return true;
//     } catch (const std::exception &e) {
//         std::cerr << "Exception: " << e.what() << std::endl;
//         return false;
//     }
// }

bool Server::joinChannel(ClientHandler *newClient, std::string &name) {
    try {
        if (checkIfChannelExists(name)) {
            std::cout << "[DEBUG] Channel already exists, adding user" << std::endl;

            Channel &existingChannel = getChannel(name);
            existingChannel.addClient(newClient);
            return true;
        }
        std::cout << "[DEBUG] Creating new channel: " << name << std::endl;
        Channel newChannel(name, newClient);
        m_channels.push_back(newChannel);
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}


bool Server::checkNickname(const std::string &nickname) {
    for (std::vector<ClientHandler *>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getUser().getNickname() == nickname) {
            return true;
        }
    }
    return false;
}

bool Server::getUserByNickname(const std::string &nickname, ClientHandler *&client_handler) {
    for (std::vector<ClientHandler *>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getUser().getNickname() == nickname) {
            client_handler = *it;
            return true;
        }
    }
    return false;
}


const std::string &Server::getServerName() const { return m_serverName; }
