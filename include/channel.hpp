#pragma once
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include "User.hpp"


class Channel {
private:
    std::vector<User> m_users;
    std::string m_name;
    std::string m_topic;
    const User &m_owner;

public:
    Channel(const std::string &name, const User &owner);
    ~Channel();
    std::vector<User> getUsers() const;
    void addUser(const User &user);
    void removeUser(const User &user);
    std::string getName() const;
    void setName(const std::string &name);
    std::string getTopic() const;
    void setTopic(const std::string &topic);
};
