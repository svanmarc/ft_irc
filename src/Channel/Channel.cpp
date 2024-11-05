#include "Channel.hpp"
Channel::Channel(const std::string &name, ClientHandler *owner) :
    m_name(name), m_owner(owner), m_inviteOnly(false), m_topicProtection(false), m_password(""), m_userLimit(0) {
    m_topic = "";
    m_clients.push_back(owner);
    m_operators.push_back(owner);
}

Channel::~Channel() {}

std::vector<ClientHandler *> Channel::getClients() const { return m_clients; }

void Channel::addClient(ClientHandler *client) {
    if (checkIfClientIsInChannel(client)) {
        std::cerr << RED << "User already in channel" << RESET << std::endl;
        return;
    }
    m_clients.push_back(client);
}
void Channel::removeClient(const ClientHandler *client) {
    std::vector<ClientHandler *>::iterator it = std::find(m_clients.begin(), m_clients.end(), client);

    if (it != m_clients.end()) {
        m_clients.erase(it);
    }
}


std::string Channel::getName() const { return m_name; }

void Channel::setName(const std::string &name) { this->m_name = name; }

std::string Channel::getTopic() const { return m_topic; }

void Channel::setTopic(const std::string &topic) { this->m_topic = topic; }

bool Channel::checkIfClientIsInChannel(ClientHandler *client) const {
    for (std::vector<ClientHandler *>::const_iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        if (*it == client) {
            return true;
        }
    }
    return false;
}

void Channel::printAllNicknamesInChannel() const {
    std::cout << BLUE << "Nicknames in channel " << m_name << ": ";
    for (std::vector<ClientHandler *>::const_iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        std::cout << MAGENTA << (*it)->getNickname() << " ";
    }
    std::cout << RESET << std::endl;
}

int Channel::getNumberOfClients() const { return m_clients.size(); }
