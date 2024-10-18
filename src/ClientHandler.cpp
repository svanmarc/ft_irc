#include "ClientHandler.hpp"
#include "CommandHandler.hpp"

ClientHandler::ClientHandler(int const socket, Server *serverRef) :
    m_clientSocket(socket), m_server(serverRef), m_attempts(0) {}

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
    std::cout << "Reading command: " << command << std::endl;
    std::istringstream commandStream(command);
    std::string singleCommand;

    while (std::getline(commandStream, singleCommand, '\n')) {
        // Supprimer '\r' à la fin de la commande, s'il est présent
        if (!singleCommand.empty() && singleCommand[singleCommand.size() - 1] == '\r') {
            singleCommand.erase(singleCommand.size() - 1);
        }

        // Si la commande n'est pas vide, on la traite
        if (!singleCommand.empty()) {
            std::cout << "Processing command: " << singleCommand << std::endl;

            // Gestion de l'authentification
            if (!this->getUser().isAuthenticated()) {
                std::cout << "User not authenticated" << std::endl;
                commandHandler.handleCommandNoAuth(singleCommand, this);
                continue; // Passer à la prochaine commande sans quitter la boucle
            }

            // Gestion de l'enregistrement
            if (!this->getUser().isRegistered()) {
                std::cout << "User not registered" << std::endl;
                commandHandler.handleCommandNoRegistred(singleCommand, this);
                continue; // Passer à la prochaine commande
            }

            // Si l'utilisateur est authentifié et enregistré
            std::cout << "User authenticated and registered" << std::endl;
            commandHandler.handleCommand(singleCommand, this);
        }
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

bool ClientHandler::operator==(const ClientHandler &other) const {
    return (this->m_clientSocket == other.m_clientSocket);
}
