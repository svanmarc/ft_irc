#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

ClientHandler::ClientHandler(int const socket, Server *serverRef) :
    m_clientSocket(socket), m_server(serverRef), m_attempts(0), m_isOperator(false), m_isInvited(false) {}

ClientHandler::~ClientHandler() { close(m_clientSocket); }

void ClientHandler::handlerClient(Server &server) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = read(m_clientSocket, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        // Gestion de l'erreur de lecture
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << RED << "Error reading from client socket: " << strerror(errno) << RESET << std::endl;
            server.handleClientDisconnect(m_clientSocket);
        }
    } else if (bytesRead == 0) {
        // Le client s'est déconnecté proprement
        std::cerr << RED << "Client disconnected." << RESET << std::endl;
        server.handleClientDisconnect(m_clientSocket);
    } else {
        // Lecture réussie, traiter la commande reçue
        m_commandPartial += std::string(buffer, bytesRead);
        readCommand();
    }
}
void ClientHandler::readCommand() {
    CommandHandler commandHandler(*m_server);
    std::istringstream commandStream(m_commandPartial);
    std::string singleCommand;
    if (m_commandPartial[m_commandPartial.size() - 1] != '\n')
        return;
    while (std::getline(commandStream, singleCommand, '\n')) {
        // Supprimer '\r' à la fin de la commande
        if (!singleCommand.empty() && singleCommand[singleCommand.size() - 1] == '\r') {
            singleCommand.erase(singleCommand.size() - 1);
        }

        if (!singleCommand.empty()) {
            if (!this->getUser().isRegistered()) {
                commandHandler.handleCommandNoRegister(singleCommand, this);
                continue;
            }
            commandHandler.handleCommandRegister(singleCommand, this);
        }
    }
    if (!m_commandPartial.empty() && m_commandPartial[m_commandPartial.size() - 1] == '\n')
            m_commandPartial.erase();

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
