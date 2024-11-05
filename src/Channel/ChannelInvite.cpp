#include "Channel.hpp"


void Channel::inviteClient(ClientHandler *client) {
    if (isClientInvited(client)) {
        std::cerr << RED << "Client " << client->getNickname() << " is already invited to channel " << m_name << RESET
                  << std::endl;
        return;
    }
    m_invitedClients.push_back(client);
    std::cout << GREEN << "Client " << client->getNickname() << " invited to channel " << m_name << RESET << std::endl;
    client->addChannelToList(this->getName());
}


bool Channel::isClientInvited(ClientHandler *client) const {
    return std::find(m_invitedClients.begin(), m_invitedClients.end(), client) != m_invitedClients.end();
}

void Channel::removeInvitedClient(ClientHandler *client) {
    std::vector<ClientHandler *>::iterator it = std::find(m_invitedClients.begin(), m_invitedClients.end(), client);
    if (it != m_invitedClients.end()) {
        m_invitedClients.erase(it);
        std::cout << GREEN << "Client " << client->getNickname() << " removed from invited list of channel " << m_name
                  << RESET << std::endl;
    }
}
