#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

ClientHandler::ClientHandler(int const socket, Server *serverRef) :
    m_clientSocket(socket), m_server(serverRef), m_attempts(0), m_isOperator(false), m_isInvited(false) {}

ClientHandler::~ClientHandler() {
    close(m_clientSocket);
    std::cout << "Socket " << m_clientSocket << " closed" << std::endl;
}

void ClientHandler::handlerClient(Server &server) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = read(m_clientSocket, buffer, sizeof(buffer) - 1);

    if (bytesRead < 0) {
        // Gestion de l'erreur de lecture
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Error reading from client socket: " << strerror(errno) << std::endl;
            server.handleClientDisconnect(m_clientSocket);
        }
    } else if (bytesRead == 0) {
        // Le client s'est déconnecté proprement
        std::cerr << "Client disconnected." << std::endl;
        server.handleClientDisconnect(m_clientSocket);
    } else {
        // Lecture réussie, traiter la commande reçue
        std::string command(buffer);
        std::cout << "Received command: " << command << std::endl;
        readCommand(command);
    }
}

void ClientHandler::readCommand(const std::string &command) {
    CommandHandler commandHandler(*m_server);
    std::cout << "Reading command: " << command << std::endl;
    std::istringstream commandStream(command);
    std::string singleCommand;

    while (std::getline(commandStream, singleCommand, '\n')) {
        // Supprimer '\r' à la fin de la commande
        if (!singleCommand.empty() && singleCommand[singleCommand.size() - 1] == '\r') {
            singleCommand.erase(singleCommand.size() - 1);
        }

        if (!singleCommand.empty()) {
            std::cout << "Processing command: " << singleCommand << std::endl;

            // Gestion de l'enregistrement
            if (!this->getUser().isRegistered()) {
                std::cout << "🚫----- User not registered ----" << std::endl;
                commandHandler.handleCommandNoRegister(singleCommand, this);
                continue;
            }

            // Si l'utilisateur est authentifié et enregistré
            std::cout << "🔑----- User authenticated and registered" << std::endl;
            commandHandler.handleCommandRegister(singleCommand, this);
        }
    }
}

// Méthode pour obtenir le socket du client
int ClientHandler::getSocket() const { return m_clientSocket; }

// Méthodes pour la gestion de l'authentification
bool ClientHandler::isAuthenticated() const { return m_user.isAuthenticated(); }

void ClientHandler::setAuthenticated(bool isAuthenticated) { m_user.setAuthenticated(isAuthenticated); }

int ClientHandler::getAttempts() const { return m_attempts; }

void ClientHandler::incrementAttempts() { m_attempts++; }

void ClientHandler::resetAttempts() { m_attempts = 0; }

// Gestion des informations de l'utilisateur
User &ClientHandler::getUser() { return m_user; }

const std::string &ClientHandler::getNickname() const { return m_user.getNickname(); }

void ClientHandler::setNickname(const std::string &nickname) { m_user.setNickname(nickname); }

// Gestion des canaux
void ClientHandler::addChannelToList(const std::string &channel) {
    if (std::find(channels.begin(), channels.end(), channel) == channels.end()) {
        channels.push_back(channel);
    }
}

void ClientHandler::leaveChannel(const std::string &channel) {
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (*it == channel) {
            channels.erase(it);
            std::cout << "Client " << getNickname() << " a quitté le canal " << channel << std::endl;
            break;
        }
    }
}

std::vector<std::string> ClientHandler::getChannels() const { return channels; }

// Méthode pour obtenir le serveur associé
Server *ClientHandler::getServer() const { return m_server; }

// Surcharge de l'opérateur == pour comparer deux clients
bool ClientHandler::operator==(const ClientHandler &other) const {
    return (this->m_clientSocket == other.m_clientSocket);
}

// Gestion des privilèges
bool ClientHandler::isOperator(const std::string &channel) const {
    return m_isOperator && std::find(channels.begin(), channels.end(), channel) != channels.end();
}

void ClientHandler::setOperator(bool isOperator) { m_isOperator = isOperator; }

bool ClientHandler::isInvited(const std::string &channel) const {
    return m_isInvited && std::find(channels.begin(), channels.end(), channel) != channels.end();
}

void ClientHandler::setInvited(bool isInvited) { m_isInvited = isInvited; }
