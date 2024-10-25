#include "Channel.hpp"

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