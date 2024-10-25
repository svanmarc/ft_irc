#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ClientHandler.hpp"
#include "Irc.hpp"

class Channel {
private:
    std::vector<ClientHandler *> m_clients;
    std::vector<ClientHandler *> m_invitedClients; // Liste des clients invités
    std::string m_name;
    std::string m_topic;
    ClientHandler *m_owner; // Propriétaire du canal
    std::vector<ClientHandler *> m_operators; // Liste des opérateurs

    // Attributs des modes
    bool m_inviteOnly; // +i (invite only)
    bool m_topicProtection; // +t (topic protection)
    std::string m_password; // +k (password)
    bool m_operatorPrivileges; // +o (operator privileges)
    int m_userLimit; // +l (user limit)

public:
    Channel(const std::string &name, ClientHandler *owner);
    ~Channel();

    // Gestion des clients
    std::vector<ClientHandler *> getClients() const;
    void addClient(ClientHandler *client);
    void removeClient(const ClientHandler *client);
    bool checkIfClientIsInChannel(ClientHandler *client) const;
    void printAllNicknamesInChannel() const;

    // Gestion des opérateurs
    void addOperator(ClientHandler *client);
    void removeOperator(ClientHandler *client);
    bool checkIfClientIsOperator(ClientHandler *client) const;

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
    void setInviteOnly(bool inviteOnly);
    bool getInviteOnly() const;
    void setTopicProtection(bool topicProtection);
    bool getTopicProtection() const;
    void setPassword(const std::string &password);
    std::string getPassword() const;
    void setOperatorPrivileges(bool operatorPrivileges);
    bool getOperatorPrivileges() const;
    void setUserLimit(int userLimit);
    int getUserLimit() const;
    void removeUserLimit();
};

#endif // CHANNEL_HPP
