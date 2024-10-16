#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <cerrno>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include "User.hpp"


class Server;

class ClientHandler {
public:
    ClientHandler(int socket, Server *serverRef);
    ~ClientHandler();

    void handlerClient(Server &server);

    // Gestion de l'authentification et des tentatives
    bool isAuthenticated() const;
    void setAuthenticated(bool isAuthenticated);
    int getAttempts() const;
    void incrementAttempts();
    void resetAttempts();

    // Gestion des informations de l'utilisateur
    User &getUser();
    const std::string &getNickname() const;
    void setNickname(const std::string &nickname);

    // Communication avec le client
    int getSocket() const;
    void readCommand(const std::string &command);

    void leaveChannel(const std::string &channel);
    Server *getServer() const;
    bool operator==(const ClientHandler &other) const;

private:
    int m_clientSocket;
    Server *m_server;
    int m_attempts;
    User m_user;
    std::vector<std::string> channels;
};

#endif
