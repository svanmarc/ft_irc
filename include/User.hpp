#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <ctime>

class User
{
public:
	User();
	User(const std::string &nickname, const std::string &userMode, const std::string &username, const std::string &realname, const std::string &hostname);
	~User();

	const std::string &getNickname() const;
	void setNickname(const std::string &nickname);

	const std::string &getUserMode() const;
	void setUserMode(const std::string &userMode);

	const std::string &getUsername() const;
	void setUsername(const std::string &username);

	const std::string &getRealname() const;
	void setRealname(const std::string &realname);

	const std::string &getHostname() const;
	void setHostname(const std::string &hostname);

	const std::string &getServername() const;
	void setServername(const std::string &servername);
	void setIsRegistered(const bool isRegistered);
	bool isRegistered() const;

	bool isConnected() const;
	void setConnected(const bool isConnected);

	const std::string &getPassword() const;
	void setPassword(const std::string &password);
	bool isAuthenticated() const;
	void setAuthenticated(bool authenticated);

private:
	std::string m_nickname;
	std::string m_userMode;
	std::string m_username;
	std::string m_realname;
	std::string m_hostname;
	std::string m_servername;
	std::string m_password;
	bool m_isAuthenticated;
	std::time_t m_connectionTime;
	std::time_t m_lastActivityTime;
	bool m_isRegistered;
	bool m_isConnected;
};

#endif
