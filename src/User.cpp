#include "User.hpp"

User::User()
{
}

User::User(const std::string& nickname, const std::string& userMode, const std::string& username, const std::string& realname, const std::string& hostname)
	: m_nickname(nickname), m_userMode(userMode), m_username(username), m_realname(realname), m_hostname(hostname)
{
}

User::~User()
{
}

const std::string& User::getNickname() const
{
	return m_nickname;
}

void User::setNickname(const std::string& nickname)
{
	m_nickname = nickname;
}

const std::string& User::getUserMode() const
{
	return m_userMode;
}

void User::setUserMode(const std::string& userMode)
{
	m_userMode = userMode;
}

const std::string& User::getUsername() const
{
	return m_username;
}

void User::setUsername(const std::string& username)
{
	m_username = username;
}

const std::string& User::getRealname() const
{
	return m_realname;
}

void User::setRealname(const std::string& realname)
{
	m_realname = realname;
}

const std::string& User::getHostname() const
{
	return m_hostname;
}

void User::setHostname(const std::string& hostname)
{
	m_hostname = hostname;
}

bool User::isConnected() const
{
	return m_isConnected;
}

void User::setConnected(const bool isConnected)
{
	m_isConnected = isConnected;
}

