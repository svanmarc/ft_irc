#pragma once
#define CLIENT_HPP

#include <ctime>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <vector>
#include "User.hpp"

class Server;

class ClientHandler {
public:
    ClientHandler(int socket, Server *serverRef);
    ~ClientHandler();

    bool joinChannel(const std::string &channel);
    void leaveChannel(const std::string &channel);
    void handlerClient();
    User user;

private:
    int m_clientSocket;
    Server *m_server;
    std::vector<std::string> channels;
    void readCommand(const std::string &command);
    void sendResponse(const std::string &response);
};
