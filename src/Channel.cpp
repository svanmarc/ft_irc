#include "Channel.hpp"

Channel::Channel( const std::string& name, const User& owner)
	:  m_name(name),m_owner(owner)
{
}
	
Channel::~Channel()
{
}

std::vector<User> Channel::getUsers() const
{
	return m_users;
}

void Channel::addUser(const User& user)
{
	m_users.push_back(user);
}
void Channel::removeUser(const User& user)
{
	for (size_t i = 0; i < m_users.size(); i++)
	{
		if (m_users[i] == user)
		{
			m_users.erase(m_users.begin() + i);
			break;
		}
	}
}

std::string Channel::getName() const
{
	return m_name;
}

void Channel::setName(const std::string& name)
{
	this->m_name = name;
}

std::string Channel::getTopic() const
{
	return m_topic;
}

void Channel::setTopic(const std::string& topic)
{
	this->m_topic = topic;
}