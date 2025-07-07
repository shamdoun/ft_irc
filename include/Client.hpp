#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Socket.hpp"
#include <string>

class Client
{
private:
	Socket		_clientSocket;
	std::string _nickName;
	std::string _username;
	std::string _realName;
	std::string _hostName;
	bool		_isAuthenticated;
	bool		_isRegistered;
	bool 		_hasNickname;
	bool 		_hasUsername;
	bool		_isBot;
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
	bool		getIsRegistered() const;
	bool		getHasNickname() const;
	std::string getAlteredHost() const;
	std::string getPrefix() const;
	bool		getHasUser() const;
	bool		getIsBot() const;
	void		setNickName(std::string &value);
	void		setRealName(std::string &value);
	void		setUserName(std::string &value);
	void		setClientSocket(Socket	&clientSocket);
	void		setHasNickname();
	void		setHasUserame();
	void		setHostName(std::string &value);
	void 		setIsAuthenticated(bool b);
	void 		setIsRegistered(bool b);
	void		setIsBot(bool b);
	bool		operator==(const Client& other) const;
};
#endif