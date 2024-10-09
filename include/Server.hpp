#ifndef SERVER_HPP
#define SERVER_HPP
#include "Channel.hpp"
#include "ClientHandler.hpp"
#include "Irc.hpp"

class CommandHandler;

class Server {
public:
    Server(int port, const std::string &password);
    ~Server();

    void start();
    void stop();
    void handleClientDisconnect(int clientSocket);
    bool joinChannel(const User &newUser, const std::string &name);
    bool checkIfChannelExists(const std::string &name) const;
    Channel getChannel(const std::string &name);
    // fichier Server.cpp
    bool getUserByNickname(const std::string &nickname, ClientHandler *&client_handler);
    const std::string &getServerName() const;
    const std::vector<ClientHandler *> &getClients() const { return clients; }
    bool authenticate(const std::string &clientPassword) const;


private:
    int m_serverSocket;
    sockaddr_in m_serverAddress;
    std::string m_password;
    std::vector<ClientHandler *> clients;
    std::vector<struct pollfd> fds;
    CommandHandler *commandHandler;
    std::vector<Channel> m_channels;
    std::string m_serverName;

    // Fonctions pour les sockets
    void setupSocket(int port);
    int setSocketOptions() const;
    int bindSocket(int port);
    static int createSocket();

    // fonctions pour les clients
    void acceptClient();
    void handleClient(int clientSocket);
    void removeClient(int clientSocket);
    bool checkPassword(const std::string &clientPassword) const;
    ClientHandler *findClient(int clientSocket) const;
};

#endif
