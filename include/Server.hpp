#pragma once
#define SERVER_HPP

#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "Channel.hpp"
#include "ClientHandler.hpp"
class Server {
public:
    Server(int port);
    ~Server();
    void start();
    void stop();
    bool joinChannel(const User &newUser, const std::string &name);
    bool checkIfChannelExists(const std::vector<Channel> channels, const std::string &name);
    Channel getChannel(const std::string &name);

private:
    int m_serverSocket;
    int m_port;
    sockaddr_in m_serverAddress;
    std::vector<ClientHandler *> m_clients;
    std::vector<Channel> m_channels;

    void setupSocket(int port);
    void acceptClient();
};
