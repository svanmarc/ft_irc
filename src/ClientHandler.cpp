#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

ClientHandler::ClientHandler(int const socket, Server *serverRef) :
    m_clientSocket(socket), m_server(serverRef), m_attempts(0) {}

ClientHandler::~ClientHandler() { close(m_clientSocket); }

void ClientHandler::handlerClient(Server &server) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = read(m_clientSocket, buffer, sizeof(buffer) - 1);

    if (bytesRead < 0) {
        // Gestion de l'erreur de lecture
        if (errno != EAGAIN && errno != EWOULDBLOCK) // Ignore les erreurs non critiques
        {
            std::cerr << "Error reading from client socket: " << strerror(errno) << std::endl;
            server.handleClientDisconnect(m_clientSocket); // Utilise la méthode intermédiaire
        }
    } else if (bytesRead == 0) {
        // Le client s'est déconnecté proprement
        std::cerr << "Client disconnected." << std::endl;
        server.handleClientDisconnect(m_clientSocket); // Utilise la méthode intermédiaire
    } else {
        // Lecture réussie, traiter la commande reçue
        std::string command(buffer);
        std::cout << "Received command: " << command << std::endl;
        readCommand(command);
    }
}

int ClientHandler::getSocket() const { return m_clientSocket; }
bool ClientHandler::isAuthenticated() const { return m_user.isAuthenticated(); }
void ClientHandler::setAuthenticated(bool isAuthenticated) { m_user.setAuthenticated(isAuthenticated); }
int ClientHandler::getAttempts() const { return m_attempts; }
void ClientHandler::incrementAttempts() { m_attempts++; }
void ClientHandler::resetAttempts() { m_attempts = 0; }
const std::string &ClientHandler::getNickname() const { return m_user.getNickname(); }
void ClientHandler::setNickname(const std::string &nickname) { m_user.setNickname(nickname); }
User &ClientHandler::getUser() { return m_user; }
Server *ClientHandler::getServer() const { return m_server; }

void ClientHandler::readCommand(const std::string &command) {
    CommandHandler commandHandler(*m_server);
    if (!this->getUser().isAuthenticated()) {
        std::cout << "User not authenticated" << std::endl;
        commandHandler.handleCommandNoAuth(command, this);
        return;
    }
    if (!this->getUser().isRegistered()) {
        std::cout << "User no registered" << std::endl;
        commandHandler.handleCommandNoRegistred(command, this);
        return;
    }
    std::cout << "User authenticated and registrerd" << std::endl;
    commandHandler.handleCommand(command, this);
}
bool ClientHandler::joinChannel(const std::string &channel) const {
    return (m_server->joinChannel(this->m_user, channel));
}
void ClientHandler::leaveChannel(const std::string &channel) {
    for (size_t i = 0; i < channels.size(); i++) {
        if (channels[i] == channel) {
            channels.erase(channels.begin() + i);
            break;
        }
    }
}