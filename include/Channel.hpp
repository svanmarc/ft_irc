#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ChannelMode.hpp"
#include "ClientHandler.hpp"
#include "Irc.hpp"

class Channel {
private:
    std::vector<ClientHandler *> m_clients;
    std::vector<ClientHandler *> m_invitedClients; // Liste des clients invités
    std::string m_name;
    std::string m_topic;
    ClientHandler *m_owner; // Propriétaire du canal
    ChannelMode m_mode; // Mode du canal

public:
    Channel(const std::string &name, ClientHandler *owner);
    ~Channel();

    // Gestion des clients
    std::vector<ClientHandler *> getClients() const;
    void addClient(ClientHandler *client);
    void removeClient(const ClientHandler *client);
    bool checkIfClientIsInChannel(ClientHandler *client) const;
    void printAllNicknamesInChannel() const;

    // Gestion des invitations
    void inviteClient(ClientHandler *client);
    bool isClientInvited(ClientHandler *client) const;
    void removeInvitedClient(ClientHandler *client);

    // Gestion des informations du canal
    std::string getName() const;
    void setName(const std::string &name);
    std::string getTopic() const;
    void setTopic(const std::string &topic);

    // Gestion des modes du canal
    void setMode(const ChannelMode &mode);
    void setInviteOnly(bool inviteOnly);
    bool getInviteOnly() const;
    ChannelMode getMode() const;
};

#endif // CHANNEL_HPP
