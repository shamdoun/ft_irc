#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Server_utils.hpp"
#include "../include/Authentification.hpp"
#include <algorithm>

std::vector<pollfd> Server::_pfds;

Server::Server():_name("default"),_password("defaut"),_port(8080)
{}

Server::Server(std::string name, std::string password, unsigned short port):_name(name), _password(password), _port(port)
{

}

Server::~Server()
{
	_allClients.clear();
}

void Server::setupServer()
{
	struct pollfd pfd;
	int opt = 1;
	int ret;

	_serverSocket.setSocketAdress(AF_INET, _port, INADDR_ANY);
	ret = setsockopt(_serverSocket.getSocketFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (ret < 0)
		throw std::runtime_error("failed to set options for server socket!");
	ret = bind(_serverSocket.getSocketFd(), (struct sockaddr*)&_serverSocket.getSocketAddress(), sizeof(_serverSocket.getSocketAddress()));
	if (ret < 0)
		throw std::runtime_error("failed to bind the server socket to a local address!");
	_serverSocket.setIpAddress(inet_ntoa(_serverSocket.getSocketAddress().sin_addr));
	startListening();
	pfd.fd = _serverSocket.getSocketFd();
	pfd.events = POLL_IN;
	_pfds.push_back(pfd);
}

void Server::initServer()
{
	int ret;

	setupServer();
	while (true)
	{
		ret = poll(_pfds.data(), _pfds.size(), -1);
		if (!ret)
			throw std::runtime_error("Timeout for poll, no file descriptors have been selected!");
		if (ret < 0)
			throw std::runtime_error("Poll system call has failed!");
		if (_pfds[0].revents & POLL_IN)
		{
			acceptConnection();
		}
		for (size_t i = 1; i < _pfds.size(); i++)
		{
			if (_pfds[i].revents & POLL_IN)
				receiveData(i);
		}
	}
}

void Server::startListening()
{
	int ret;

	ret = listen(_serverSocket.getSocketFd(), MAX_CONNECTIONS);
	if (ret < 0)
		throw std::runtime_error("failed to listen for connections!");
	std::cout << INIT ;
	std::cout << "Server is listening on " << GREEN_P << _serverSocket.getIpAddress() << ":"  << _port << GREEN_S  << std::endl;
}

void Server::receiveData(int i)
{
	ssize_t bytes;
	Client *c;

	c = getClientByFd(_pfds[i].fd);
	std::memset(_buffer, 0, sizeof(_buffer));
	bytes = recv(_pfds[i].fd, _buffer, sizeof(_buffer), 0);
	if (!bytes)
	{
		std::cout << "Client <" << GREEN_P << c->getId() << GREEN_S << "> has gracefully closed the connection" << std::endl;
		close(_pfds[i].fd);
		std::vector<Client>::iterator it = find(_allClients.begin(), _allClients.end(), (*c));
		_pfds.erase(_pfds.begin() + i);
		if (it != _allClients.end())
			_allClients.erase(it);
	}
	if (bytes < 0)
		throw std::runtime_error("failed to receive a new message!");
	parseMessage(_buffer, c);
}

Client *Server::getClientByFd(int fd)
{
	for (std::vector<Client>::iterator it = _allClients.begin(); it != _allClients.end(); it++)
	{
		if (it->getClientSocket().getSocketFd() == fd)
			return (&(*it));
	}
	return NULL;
}

std::vector<Client> Server::getAllClients()
{
	return _allClients;
}

Socket &Server::getServerSocket()
{
	return this->_serverSocket;
}

void Server::acceptConnection()
{
	int fd;
	struct pollfd pfd;

	Socket c(1);
	socklen_t len = sizeof(c.getSocketAddress());
	fd = accept(_serverSocket.getSocketFd(), (sockaddr *)&c.getSocketAddress(), &len);
	if (fd < 0)
		throw std::runtime_error("failed to accept a new connection!");
	if (MAX_CONNECTIONS == _allClients.size() - 2)
	{
		std::cerr << "reached maximum connections!\n";
		close(fd);
		return ;
	}
	c.setSocketFd(fd);
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("failed to make a file non-blocking");
	c.setIpAddress(inet_ntoa(c.getSocketAddress().sin_addr));
	Client newClient(c);
	newClient.setId(_allClients.size() + 1);
	_allClients.push_back(newClient);
	pfd.fd = fd;
	pfd.events = POLL_IN;
	pfd.revents = 0;
	_pfds.push_back(pfd);
	std::cout << "Client " << GREEN_P << "<" << newClient.getId() << "> " << GREEN_S << "is connected\n"; 

}

void Server::quitServer()
{
	std::cout << "\n-----------Quiting the Server-------------\n";
	std::cout << RED_P << "closing all connections...." << RED_S << std::endl;
	for (std::vector<struct pollfd>::iterator p = _pfds.begin(); p != _pfds.end(); p++)
	{
		close(p->fd);
	}
	exit(0);
}

void signalHander(int sig)
{
	(void)sig;
	Server::quitServer();
}

void Server::parseMessage(char *buf, Client *c)
{
	std::string parse(buf);
	std::vector<std::string> params;
	size_t start = 0;
	size_t end;

	parse = extractMessage(parse);
	while ((end = parse.find(' ', start)) != std::string::npos)
	{
		params.push_back(parse.substr(start, end - start));
		start = end + 1;
	}
	if (start < parse.size())
		params.push_back(parse.substr(start));
	if (params.size())
		parseParams(params, c);
	params.clear();
}

int	identifyCommand(std::string cmd)
{
	if (!cmd.compare("NICK") || !cmd.compare("nick"))
		return (0);
	if (!cmd.compare("USER")|| !cmd.compare("user"))
		return (1);
	if (!cmd.compare("PASS") || !cmd.compare("pass"))
		return (2);
	if (!cmd.compare("PRIVMSG") || !cmd.compare("privmsg"))
		return (3);
	if (!cmd.compare("JOIN") || !cmd.compare("join"))
		return (3);
	if (!cmd.compare("MODE") || !cmd.compare("mode"))
		return (3);
	if (!cmd.compare("KICK") || !cmd.compare("kick"))
		return (3);
	if (!cmd.compare("INVITE") || !cmd.compare("invite"))
    	return (3);
	if (!cmd.compare("PONG") || !cmd.compare("QUIT"))
		return (4);
	if (!cmd.compare("TOPIC") || !cmd.compare("topic"))
		return (3);
  return (-1);
} 

void Server::parseParams(std::vector<std::string> &params, Client *c)
{
	int cmd;

	cmd = identifyCommand(params[0]);
	switch (cmd)
	{
		case 0:
			handleNickNameCommand(params, c);
			break;
		case 1:
			handleUserCommand(params, c);
			break ;
		case 2:
			handlePassCommand(params, c);
			break ;
		case 3:
			Server::Commands(params, c);
			break ;
		case 4:
			return ;
		default:
			handleUnkownCommand(params[0], c);
	}
}
