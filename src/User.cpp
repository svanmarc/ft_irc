#include "User.hpp"

User::User() {
    m_isRegistered = false;
    m_isConnected = false;
    m_isAuthenticated = false;
    m_connectionTime = std::time(0);
    m_lastActivityTime = std::time(0);
}
User::User(const std::string &nickname, const std::string &userMode, const std::string &username,
           const std::string &realname, const std::string &hostname) :
    m_nickname(nickname), m_userMode(userMode), m_username(username), m_realname(realname), m_hostname(hostname) {
    m_isRegistered = false;
    m_isConnected = false;
    m_isAuthenticated = false;
    m_connectionTime = std::time(0);
    m_lastActivityTime = std::time(0);
}
User::~User() {}
const std::string &User::getNickname() const { return m_nickname; }
void User::setNickname(const std::string &nickname) { m_nickname = nickname; }
const std::string &User::getUserMode() const { return m_userMode; }
void User::setUserMode(const std::string &userMode) { m_userMode = userMode; }
const std::string &User::getUsername() const { return m_username; }
const std::string &User::getRealname() const { return m_realname; }
const std::string &User::getHostname() const { return m_hostname; }
bool User::isConnected() const { return m_isConnected; }
void User::setConnected(const bool isConnected) {
    m_connectionTime = std::time(0);
    m_lastActivityTime = std::time(0);
    m_isConnected = isConnected;
}
void User::setIsRegistered(const bool isRegistered) { m_isRegistered = isRegistered; }
bool User::isRegistered() const { return m_isRegistered; }
const std::string &User::getServername() const { return m_servername; }
bool User::operator==(const User &other) const {
    return m_nickname == other.m_nickname && m_userMode == other.m_userMode && m_username == other.m_username &&
           m_realname == other.m_realname && m_hostname == other.m_hostname && m_servername == other.m_servername &&
           m_isRegistered == other.m_isRegistered && m_isConnected == other.m_isConnected;
}
bool User::isAuthenticated() const { return m_isAuthenticated; }
void User::setAuthenticated(bool isAuthenticated) { m_isAuthenticated = isAuthenticated; }
void User::Register(const std::string &nickname, const std::string &userMode, const std::string &username,
                    const std::string &realname, const std::string &hostname, const std::string &servername) {
    m_nickname = nickname;
    m_userMode = userMode;
    m_username = username;
    m_realname = realname;
    m_hostname = hostname;
    m_servername = servername;
    m_isRegistered = true;
    m_isConnected = true;
    m_connectionTime = std::time(0);
    m_lastActivityTime = std::time(0);
}
