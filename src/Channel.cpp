#include "Channel.hpp"

Channel::Channel(const std::string &name, ClientHandler *owner) : m_name(name), m_owner(owner) {
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
    for (std::vector<ClientHandler *>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        if (*it == client) {
            m_clients.erase(it);
            break;
        }
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
