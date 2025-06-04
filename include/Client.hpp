#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Socket.hpp"

class Client
{
private:
	Socket		_clientSocket;
	std::string _nickName;
	std::string _username;
	std::string _realName;
	std::string _hostName;
	// std::string _serverName;
	// std::string	_password;
	bool		_isAuthenticated;
	bool 		_hasNickname;
public:
	Client();
	Client(Socket _clientSocket);
	~Client();
	void		disconnectFromServer();
	Socket		&getClientSocket();
	std::string getNickName() const;
	std::string getUserName() const;
	std::string getRealName() const;
	std::string getHostName() const;
	bool		getIsAuthenticated() const;
	// std::string	getPassword() const;
	bool		getHasNickname() const;
	// void		setPassword(std::string &value);
	void		setNickName(std::string &value);
	void		setRealName(std::string &value);
	void		setUserName(std::string &value);
	void		setClientSocket(Socket	&clientSocket);
	void		setHasNickname();
	void		setHostName(std::string &value);
	void 		setIsAuthenticated(bool b);
};
#endif