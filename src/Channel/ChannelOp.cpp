#include "Channel.hpp"

void Channel::addOperator(ClientHandler *client) {
    if (checkIfClientIsOperator(client)) {
        if (client != m_owner) {
            std::cerr << RED << "Client " << client->getNickname() << " is already an operator of channel " << m_name
                      << RESET << std::endl;
        }
        return;
    }
    m_operators.push_back(client);
    client->setOperator(true);
    if (!isClientInvited(client)) {
        inviteClient(client);
    }
}

void Channel::removeOperator(ClientHandler *client) {
    if (client == m_owner) {
        std::cerr << RED << "Client " << client->getNickname() << " is the owner of channel " << m_name << RESET
                  << std::endl;
        return;
    }
    std::vector<ClientHandler *>::iterator it = std::find(m_operators.begin(), m_operators.end(), client);
    if (it != m_operators.end()) {
        m_operators.erase(it);
        client->setOperator(false);
        if (isClientInvited(client)) {
            removeInvitedClient(client);
        }
    } else {
        std::cerr << RED << "Client " << client->getNickname() << " is not an operator of channel " << m_name << RESET
                  << std::endl;
    }
}

bool Channel::checkIfClientIsOperator(ClientHandler *client) const {
    return std::find(m_operators.begin(), m_operators.end(), client) != m_operators.end();
}
