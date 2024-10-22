#include "ChannelMode.hpp"

ChannelMode::ChannelMode() {
    m_inviteOnly = false;
    m_topicProtection = false;
    m_password = "";
    m_operatorPrivileges = false;
    m_userLimit = -1;
}

ChannelMode::~ChannelMode() {
}

bool ChannelMode::getInviteOnly() const {
    return m_inviteOnly;
}

void ChannelMode::setInviteOnly(bool inviteOnly) {
    m_inviteOnly = inviteOnly;
}

bool ChannelMode::getTopicProtection() const {
    return m_topicProtection;
}

void ChannelMode::setTopicProtection(bool topicProtection) {
    m_topicProtection = topicProtection;
}