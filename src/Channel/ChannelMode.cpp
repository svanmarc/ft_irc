#include "Channel.hpp"

void Channel::setInviteOnly(bool inviteOnly) { m_inviteOnly = inviteOnly; }

bool Channel::getInviteOnly() const { return m_inviteOnly; }

void Channel::setTopicProtection(bool topicProtection) { m_topicProtection = topicProtection; }

bool Channel::getTopicProtection() const { return m_topicProtection; }

void Channel::setPassword(const std::string &password) { m_password = password; }

std::string Channel::getPassword() const { return m_password; }

// void Channel::setOperatorPrivileges(bool operatorPrivileges) { m_operatorPrivileges = operatorPrivileges; }

// bool Channel::getOperatorPrivileges() const { return m_operatorPrivileges; }

void Channel::setUserLimit(int userLimit) { m_userLimit = userLimit; }

int Channel::getUserLimit() const { return m_userLimit; }

void Channel::removeUserLimit() { m_userLimit = 0; }