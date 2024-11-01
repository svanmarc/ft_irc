#ifndef USER_HPP
#define USER_HPP

#include "Irc.hpp"

class User {
public:
    User();
    User(const std::string &nickname, const std::string &userMode, const std::string &username,
         const std::string &realname, const std::string &hostname);
    ~User();

    const std::string &getNickname() const;
    void setNickname(const std::string &nickname);
    const std::string &getUserMode() const;
    void setUserMode(const std::string &userMode);
    const std::string &getUsername() const;
    const std::string &getRealname() const;
    const std::string &getHostname() const;
    const std::string &getServername() const;
    void setIsRegistered(bool isRegistered);
    bool isRegistered() const;
    bool isConnected() const;
    void setConnected(bool isConnected);
    const std::string &getPassword() const;
    void setPassword(const std::string &password);
    bool isAuthenticated() const;
    void setAuthenticated(bool isAuthenticated);
    bool operator==(const User &other) const;
    std::time_t getConnectionTime() const;
    std::time_t getLastActivityTime() const;
    void Register(const std::string &nickname, const std::string &userMode, const std::string &username,
                  const std::string &realname, const std::string &hostname, const std::string &servername);


private:
    std::string m_nickname;
    std::string m_userMode;
    std::string m_username;
    std::string m_realname;
    std::string m_hostname;
    std::string m_servername;
    std::string m_password;
    bool m_isAuthenticated;
    bool m_isRegistered;
    bool m_isConnected;
    std::time_t m_connectionTime;
    std::time_t m_lastActivityTime;
};

#endif
