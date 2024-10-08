#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <vector>
#include "Channel.hpp"
#include "ClientHandler.hpp"

class CommandHandler;

class Server {
public:
    Server(int port, const std::string &password);
    ~Server();

    void start();
    void stop();
    bool authenticate(const std::string &clientPassword);
    void handleClientDisconnect(int clientSocket);
    bool joinChannel(const User &newUser, const std::string &name);
    bool checkIfChannelExists(const std::string &name) const;
    Channel getChannel(const std::string &name);
    const std::vector<ClientHandler *> &getClients() const { return clients; }

private:
    int m_serverSocket;
    sockaddr_in m_serverAddress;
    std::string m_password;
    std::vector<ClientHandler *> clients;
    std::vector<struct pollfd> fds;
    CommandHandler *commandHandler;
    std::vector<Channel> m_channels;

    // Fonctions pour les sockets
    void setupSocket(int port);
    int setSocketOptions() const;
    int bindSocket(int port);
    static int createSocket();

    // fonctions pour les clients
    void acceptClient();
    void handleClient(int clientSocket);
    bool checkPassword(const std::string &clientPassword);
    void removeClient(int clientSocket);
    ClientHandler *findClient(int clientSocket) const;
};

#endif
