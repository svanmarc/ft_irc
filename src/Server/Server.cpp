#include "Server.hpp"
#include "CommandHandler.hpp"


void Server::ServerWelcomMessage() {
    std::cout << "                                                        " << std::endl;
    std::cout << "                                                        " << std::endl;
    std::cout << "                                                        " << std::endl;
    std::cout << GREEN << "|   \\______   \\_   ___ \\   /   _____/\\_   _____/\\______   \\   \\ /   /" << std::endl;
    std::cout << "|   ||       _/    \\  \\/   \\_____  \\  |    __)_  |       _/\\   Y   / " << std::endl;
    std::cout << "|   ||    |   \\     \\____  /        \\ |        \\ |    |   \\ \\     /  " << std::endl;
    std::cout << "|___||____|_  /\\______  / /_______  //_______  / |____|_  /  \\___/   " << std::endl;
    std::cout << "            \\/        \\/          \\/         \\/         \\/           " << RESET << std::endl;
    std::cout << "                                                        " << std::endl;
    std::cout << "                                                        " << std::endl;
    std::cout << "                                                        " << std::endl;
    std::cout << "                                                        " << std::endl;
    std::cout << "                                                        " << std::endl;
    std::cout << "                                                        " << std::endl;
    std::cout << CYAN << "     🚦 Server is running. Waiting for connections..." << std::endl;
    std::cout << CYAN << "     🔐 Password set to: " << YELLOW << m_password << std::endl;
    std::cout << CYAN << "     👾 Port " << YELLOW << m_port << "." << std::endl;
    std::cout << CYAN << "     😘 Press Ctrl+C to stop." << RESET << std::endl;
    std::cout << "\n\n\n" << std::endl;
}

Server::Server(const int port, const std::string &password) : m_password(password), m_port(port) {
    m_serverName = "irc.svanmarc_mrabat";
    setupSocket(port);
    commandHandler = new CommandHandler(*this);
    // Ajouter le socket d'écoute à la liste des descripteurs surveillés par `poll`
    const struct pollfd serverPollFD = {m_serverSocket, POLLIN, 0};
    m_startTime = std::time(0);
    fds.push_back(serverPollFD);
}

Server::~Server() {
    if (m_serverSocket >= 0) {
        stop();
    }
    if (commandHandler != 0) {
        delete commandHandler;
        commandHandler = 0;
    }
}

void Server::start() {
    ServerWelcomMessage();
    while (true) {
        // Appeler `poll` pour surveiller les sockets de tous les clients
        int const pollCount = poll(&fds[0], fds.size(), -1);
        if (pollCount < 0) {
            std::cerr << RED << "Error in poll: " << std::strerror(errno) << RESET << std::endl;
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
    if (m_isStopped) {
        return;
    }
    m_isStopped = true;
    std::cout << BLUE << "Stopping server..." << RESET << std::endl;
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
    std::cout << BLUE << "Server stopped" << RESET << std::endl;
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

bool Server::joinChannel(ClientHandler *newClient, std::string &name) {
    try {
        if (checkIfChannelExists(name)) {
            // Récupérer une référence au canal existant et ajouter le client
            Channel &existingChannel = getChannel(name);
            if (existingChannel.checkIfClientIsInChannel(newClient)) {
                return false;
            }
            if (existingChannel.getInviteOnly() && !existingChannel.isClientInvited(newClient)) {
                MessageHandler::sendErrorInviteOnly(newClient, name);
                return false;
            }
            existingChannel.addClient(newClient);
            return true;
        }
        // Si le canal n'existe pas, créer un nouveau canal et ajouter le client
        Channel newChannel(name, newClient);
        newChannel.addOperator(newClient); // Add the client as operator
        newClient->setOperator(true); // Set the client as operator
        m_channels.push_back(newChannel);
        return true;
    } catch (const std::exception &e) {
        std::cerr << RED << "Exception: " << e.what() << RESET << std::endl;
        return false;
    }
}

std::vector<Channel> &Server::getChannels() { return m_channels; }
std::time_t Server::getStartTime() const { return m_startTime; }
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

bool Server::isRunning() const { return !m_isStopped; }
