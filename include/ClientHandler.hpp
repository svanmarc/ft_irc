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

    // Communication avec le client et gestion des commandes
    void handlerClient(Server &server);
    void readCommand(const std::string &command);

    // Gestion de l'authentification
    bool isAuthenticated() const;
    void setAuthenticated(bool isAuthenticated);
    int getAttempts() const;
    void incrementAttempts();
    void resetAttempts();

    // Gestion des informations de l'utilisateur
    User &getUser();
    const std::string &getNickname() const;
    void setNickname(const std::string &nickname);

    // Gestion des canaux
    void addChannelToList(const std::string &channel);
    void leaveChannel(const std::string &channel);
    void confirmJoinChannel(const std::string &channel);
    std::vector<std::string> getChannels() const;

    // Interaction Client-Serveur
    int getSocket() const;
    Server *getServer() const;

    bool operator==(const ClientHandler &other) const;

private:
    int m_clientSocket; // Socket du client
    Server *m_server; // Référence au serveur
    int m_attempts; // Nombre de tentatives d'authentification
    User m_user; // Informations associées à l'utilisateur
    std::vector<std::string> channels; // Liste des canaux que le client a rejoints
};

#endif
