#include "../include/Client.hpp"

Client::Client()
{
	_clientSocket = Socket(1);
}

Client::Client(Socket socket):_clientSocket(socket),_nickName("*"),_hostName("localhost"),_isAuthenticated(false),_isRegistered(false),_hasNickname(false),_hasUsername(false),_isBot(false)
{
	
}

Client::~Client()
{}

void Client::disconnectFromServer()
{
	_clientSocket.closeSocket();
}


Socket &Client::getClientSocket()
{
	return this->_clientSocket;
}

void Client::setClientSocket(Socket	&clientSocket)
{
	this->_clientSocket = clientSocket;
}

std::string Client::getNickName() const
{
	return this->_nickName;
}

std::string Client::getUserName() const
{
	return this->_username;
}

std::string Client::getHostName() const
{
	return this->_hostName;
}

std::string Client::getRealName() const
{
	return this->_realName;
}

bool Client::getHasNickname() const
{
	return this->_hasNickname;
}

bool Client::getIsAuthenticated() const
{
	return _isAuthenticated;
}

bool Client::getIsRegistered() const
{
	return _isRegistered;
}


void Client::setNickName(std::string &value)
{
	_nickName = value;
}

void Client::setIsBot(bool b)
{
	_isBot = b;
}

void Client::setUserName(std::string &value)
{
	_username = value;
}

void Client::setRealName(std::string &value)
{
	_realName = value;
}

void Client::setHostName(std::string &value)
{
	_hostName = value;
}

void Client::setIsAuthenticated(bool b)
{
	_isAuthenticated = b;
}

void Client::setHasNickname()
{
	_hasNickname = true;
}
void Client::setHasUserame()
{
	_hasUsername = true;
}

void Client::setIsRegistered(bool b)
{
	_isRegistered = b;
}

bool Client::getHasUser() const
{
	return _hasUsername;
}

std::string Client::getAlteredHost() const
{
	return (_nickName+ "!" + _username + "@");
}

std::string Client::getPrefix() const 
{
    return (_nickName+ "!" + _username + "@" + _hostName);
}

bool Client::operator==(const Client& other) const
{
	if (this->_nickName == other._nickName)
		return (true);
	return (false);
}

bool Client::getIsBot() const
{
	return _isBot;
}
