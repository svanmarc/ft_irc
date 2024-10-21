#pragma once
#define CHANNEL_HPP

#include "Irc.hpp"
#include "ClientHandler.hpp"
#include "ChannelMode.hpp"



class Channel {
private:
    std::vector<ClientHandler*> m_clients;
    std::string m_name;
    std::string m_topic;
    ClientHandler *m_owner;  // Propriétaire du canal (pointeur)
    // ChannelMode m_mode; // Mode du canal
    ChannelMode m_mode;


public:
    Channel(const std::string &name, ClientHandler *owner);
    ~Channel();
    std::vector<ClientHandler*> getClients() const;
    void addClient(ClientHandler* client);
    void removeClient(const ClientHandler* client);
    std::string getName() const;
    void setName(const std::string &name);
    std::string getTopic() const;
    void setTopic(const std::string &topic);
    bool checkIfClientIsInChannel(ClientHandler* client) const;
    void printAllNicknamesInChannel() const;
    //gestion des modes
    void setMode(const ChannelMode &mode);
    void setInviteOnly(bool inviteOnly);
    ChannelMode getMode() const;


};
