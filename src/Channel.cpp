#include "Channel.hpp"

Channel::Channel(const std::string &name, ClientHandler *owner) : m_name(name), m_owner(owner) {
    m_topic = "";
    m_clients.push_back(owner);
}

Channel::~Channel() {}

std::vector<ClientHandler *> Channel::getClients() const { return m_clients; }

void Channel::addClient(ClientHandler *client) {
    if (checkIfClientIsInChannel(client)) {
        std::cerr << "[DEBUG] Client " << client->getNickname() << " is already in channel " << m_name << std::endl;
        return;
    }
    m_clients.push_back(client);
    std::cout << "[DEBUG] Client " << client->getNickname() << " added to channel " << m_name
              << ". Total clients: " << m_clients.size() << std::endl;

    printAllNicknamesInChannel();
}


// void Channel::removeClient(const ClientHandler *client) {
//     for (std::vector<ClientHandler *>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
//         if (*it == client) {
//             m_clients.erase(it);
//             break;
//         }
//     }
// }

// void Channel::removeClient(const ClientHandler *client) {
//     std::cout << "Removing client " << client->getNickname() << " from channel " << m_name << std::endl;
//     for (std::vector<ClientHandler *>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
//         if (*it == client) {
//             m_clients.erase(it);
//             std::cout << "Client " << client->getNickname() << " removed from channel " << m_name << std::endl;
//             break;
//         }
//     }
// }

void Channel::removeClient(const ClientHandler *client) {
    std::cout << "[DEBUG] Attempting to remove client " << client->getNickname() << " from channel " << m_name
              << std::endl;

    std::vector<ClientHandler *>::iterator it = std::find(m_clients.begin(), m_clients.end(), client);

    if (it != m_clients.end()) {
        m_clients.erase(it);
        std::cout << "[DEBUG] Client " << client->getNickname() << " successfully removed from channel " << m_name
                  << std::endl;
    } else {
        std::cout << "[DEBUG] Client " << client->getNickname() << " was not found in channel " << m_name << std::endl;
    }

    printAllNicknamesInChannel();
}


std::string Channel::getName() const { return m_name; }

void Channel::setName(const std::string &name) { this->m_name = name; }

std::string Channel::getTopic() const { return m_topic; }

bool Channel::checkIfClientIsInChannel(ClientHandler *client) const {
    for (std::vector<ClientHandler *>::const_iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        if (*it == client) {
            std::cout << "[DEBUG] Client " << client->getNickname() << " is already in channel " << m_name << std::endl;
            return true;
        }
    }
    std::cout << "[DEBUG] Client " << client->getNickname() << " is not in channel " << m_name << std::endl;
    return false;
}


void Channel::printAllNicknamesInChannel() const {
    std::cout << "[DEBUG] Nicknames in channel " << m_name << ": ";
    for (std::vector<ClientHandler *>::const_iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        std::cout << (*it)->getNickname() << " ";
    }
    std::cout << std::endl;
}
