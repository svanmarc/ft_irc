#include "Channel.hpp"

Channel::Channel(const std::string &name, ClientHandler *owner) : m_name(name), m_owner(owner) {
    m_topic = "";
    m_clients.push_back(owner);
}

Channel::~Channel() {}

std::vector<ClientHandler*> Channel::getClients() const { return m_clients; }
void Channel::addClient(ClientHandler* client) {
    if (checkIfClientIsInChannel(client)) {
        std::cerr << "User already in channel" << std::endl;
        return;
    }
    m_clients.push_back(client);
}
void Channel::removeClient(const ClientHandler* client) {
    for (size_t i = 0; i < m_clients.size(); i++) {
        if (m_clients[i] == client) {
            m_clients.erase(m_clients.begin() + i);
            break;
        }
    }
}
std::string Channel::getName() const { return m_name; }
void Channel::setName(const std::string &name) { this->m_name = name; }
std::string Channel::getTopic() const { return m_topic; }

bool Channel::checkIfClientIsInChannel(ClientHandler* client) const {
    for (size_t i = 0; i < m_clients.size(); i++) {
        if (m_clients[i] == client) {
            return true;
        }
    }
    return false;
}

void Channel::printAllNicknamesInChannel() const {
    std::cout << "Nicknames in channel " << m_name << ": ";
    for (size_t i = 0; i < m_clients.size(); i++) {
        std::cout << m_clients[i]->getNickname() << " ";
    }
    std::cout << std::endl;
}

