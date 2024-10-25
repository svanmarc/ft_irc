#include "Channel.hpp"

// Channel::Channel(const std::string &name, ClientHandler *owner) : m_name(name), m_owner(owner) {
//     m_topic = "";
//     m_clients.push_back(owner);
// }

Channel::Channel(const std::string &name, ClientHandler *owner) :
    m_name(name), m_owner(owner), m_inviteOnly(false), m_topicProtection(false), m_password(""),
    m_operatorPrivileges(false), m_userLimit(0) {
    m_topic = "";
    m_clients.push_back(owner);
}


Channel::~Channel() {}

std::vector<ClientHandler *> Channel::getClients() const { return m_clients; }

void Channel::addClient(ClientHandler *client) {
    if (checkIfClientIsInChannel(client)) {
        std::cerr << "User already in channel" << std::endl;
        return;
    }
    m_clients.push_back(client);
}

void Channel::removeClient(const ClientHandler *client) {
    std::cout << "Removing client " << client->getNickname() << " from channel " << m_name << std::endl;

    // Utilisation de std::find pour simplifier la recherche
    std::vector<ClientHandler *>::iterator it = std::find(m_clients.begin(), m_clients.end(), client);

    if (it != m_clients.end()) {
        m_clients.erase(it);
        std::cout << "Client " << client->getNickname() << " removed from channel " << m_name << std::endl;
    } else {
        std::cout << "Client " << client->getNickname() << " not found in channel " << m_name << std::endl;
    }
}


std::string Channel::getName() const { return m_name; }

void Channel::setName(const std::string &name) { this->m_name = name; }

std::string Channel::getTopic() const { return m_topic; }

bool Channel::checkIfClientIsInChannel(ClientHandler *client) const {
    for (std::vector<ClientHandler *>::const_iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        if (*it == client) {
            return true;
        }
    }
    return false;
}

void Channel::printAllNicknamesInChannel() const {
    std::cout << "Nicknames in channel " << m_name << ": ";
    for (std::vector<ClientHandler *>::const_iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        std::cout << (*it)->getNickname() << " ";
    }
    std::cout << std::endl;
}

void Channel::inviteClient(ClientHandler *client) {
    if (isClientInvited(client)) {
        std::cerr << "Client " << client->getNickname() << " is already invited to channel " << m_name << std::endl;
        return;
    }
    m_invitedClients.push_back(client);
    client->addChannelToList(this->getName());
    std::cout << "Client " << client->getNickname() << " invited to channel " << m_name << std::endl;
}


bool Channel::isClientInvited(ClientHandler *client) const {
    return std::find(m_invitedClients.begin(), m_invitedClients.end(), client) != m_invitedClients.end();
}

void Channel::removeInvitedClient(ClientHandler *client) {
    std::vector<ClientHandler *>::iterator it = std::find(m_invitedClients.begin(), m_invitedClients.end(), client);
    if (it != m_invitedClients.end()) {
        m_invitedClients.erase(it);
        std::cout << "Client " << client->getNickname() << " removed from invited list of channel " << m_name
                  << std::endl;
    }
}
