#include "include/Socket.hpp"
#include <cstdlib>

Socket::Socket()
{
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd < 0)
		throw std::runtime_error("failed to create a socket file");
}

Socket::Socket(int type)
{
	if (!type)
	{
		_socketFd =  socket(AF_INET, SOCK_STREAM, 0);
		if (_socketFd < 0)
			throw std::runtime_error("failed to create a socket file");
	}
}

Socket::~Socket(){}

int	Socket::getSocketFd()
{
	return this->_socketFd;
}

std::string Socket::getIpAddress()
{
	return this->_ipAddress;

}

struct sockaddr_in &Socket::getSocketAddress()
{
	return this->_socketAdress;
}

void Socket::setSocketFd(int fd)
{
	this->_socketFd = fd;
}

void Socket::setSocketAdress(sa_family_t family, in_port_t &port, u_int32_t addr)
{
	this->_socketAdress.sin_family = family;
	this->_socketAdress.sin_port = htons(port);
	this->_socketAdress.sin_addr.s_addr = addr;
}

void Socket::setIpAddress(std::string hostName)
{
	this->_ipAddress = hostName;
}

void Socket::closeSocket()
{
	close(this->_socketFd);
}
