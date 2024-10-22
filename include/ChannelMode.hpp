#ifndef CHANNELMODE_H
#define CHANNELMODE_H
#include "Irc.hpp"

class ChannelMode {
    private:
        bool m_inviteOnly; // +i (invite only)
        bool m_topicProtection; // +t (topic protection)
        std::string m_password; // +k (password)
        bool m_operatorPrivileges; // +o (operator privileges)
        int m_userLimit; // +l (user limit)

    public:
        ChannelMode();
        ~ChannelMode();
        bool getInviteOnly() const;
        void setInviteOnly(bool inviteOnly);
        bool getTopicProtection() const;
        void setTopicProtection(bool topicProtection);
};
#endif //CHANNELMODE_H

