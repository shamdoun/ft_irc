#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <exception>
#include <arpa/inet.h>
#define BUFFER_SIZE  512

class Socket
{
	private:
		int					_socketFd;
		std::string			_ipAddress;
		struct sockaddr_in	_socketAdress;
	public:
		Socket();
		Socket(int type);
		~Socket();
		struct sockaddr_in	&getSocketAddress();
		int					getSocketFd();
		std::string 		getIpAddress();
		void				setSocketAdress(sa_family_t family, in_port_t &port, u_int32_t addr);
		void				setSocketFd(int fd);
		void				setIpAddress(std::string hostName);
		void				closeSocket();
};

#endif