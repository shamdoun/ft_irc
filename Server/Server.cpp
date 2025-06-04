#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"

std::vector<pollfd> Server::_pfds;

Server::Server():_name("default"),_password("defaut"),_port(8080)
{}

Server::Server(std::string name, std::string password, unsigned short port):_name(name), _password(password), _port(port)
{
}

Server::~Server()
{
}

void Server::setupServer()
{
	struct pollfd pfd;
	int opt = 1;
	int ret;


	_serverSocket = Socket();
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
	std::cout << "Server is listening on " << _serverSocket.getIpAddress() << ":"  << _port << std::endl;
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
		std::cout << "client has gracefully closed the connection" << std::endl;
		close(_pfds[i].fd);
		_pfds.erase(_pfds.begin() + i);
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
	c.setSocketFd(fd);
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("failed to make a file non-blocking");
	c.setIpAddress(inet_ntoa(c.getSocketAddress().sin_addr));
	Client newClient(c);
	_allClients.push_back(newClient);

	pfd.fd = fd;
	pfd.events = POLL_IN;
	pfd.revents = 0;
	_pfds.push_back(pfd);
	std::cout << "new client is connected on host: "  << c.getIpAddress() << std::endl;
}

void Server::quitServer()
{
	std::cout << "closing all connections" << std::endl;
	for (std::vector<struct pollfd>::iterator p = _pfds.begin(); p != _pfds.end(); p++)
	{
		close(p->fd);
	}
	exit(-1);
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

	std::cout << "len of message" << parse.length() << std::endl;
	parse = extractMessage(parse);
	std::cout << "len of message after" << parse.length() << std::endl;
	std::cout << "params size before " << params.size() << std::endl;
	while ((end = parse.find(" ", start)) != std::string::npos)
	{
		params.push_back(parse.substr(start, end - start));
		start = end + 1;
	}
	if (start < parse.size())
		params.push_back(parse.substr(start));
	std::cout << "params size after " << params.size() << std::endl;
	for (size_t i = 0; i < params.size(); i++)
	{
		std::cout << "value is " << params[i] << std::endl;
	}
	if (params.size() && params[0] != "QUIT")
	{
		std::cout << "handling " << params[0] << std::endl;
		parseParams(params, c);
	}
	params.clear();
	// std::cout << "nick name: " << c->getNickName() << " username: " << c->getUserName() << std::endl;
}

int	identifyCommand(std::string cmd)
{
	if (!cmd.compare("NICK"))
		return (0);
	if (!cmd.compare("USER"))
		return (1);
	if (!cmd.compare("PASS"))
		return (2);
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
		default:
			std::cout << "unknown command " << cmd << std::endl;
	}
}

void Server::handlePassCommand(std::vector<std::string> &params, Client *c)
{
	//handle errors
	std::string err;
	if (c->getIsAuthenticated())
	{
		err = ERR_ALREADYREGISTERED(c->getNickName());
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
	}
	else if (params.size() < 2)
	{
		err = ERR_NEEDMOREPARAMS(c->getNickName(), "PASS");
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
	}
	else if (params[1] != _password)
	{
		err = ERR_PASSMISMATCH(c->getNickName());
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
	}
	else
		c->setIsAuthenticated(true);
}

int isValidNickname(std::string const &nick)
{
	size_t i = 0;
	std::string specialChars("[]\\`_^{|}");
	if (nick.length() > 9)
		return (0);
	if (!std::isalpha(nick[0]) && (specialChars.find(nick[0]) == std::string::npos))
		return (0);
	i++;
	while (i < nick.length())
	{
		if (!std::isalnum(nick[i]) && (specialChars.find(nick[i]) == std::string::npos))
			return (0);
		i++;
	}
	return (1);
}

int Server::nickNameAlreadyExists(std::string const &nick)
{
	std::vector<Client>::iterator it;

	it = _allClients.begin();
	while (it != _allClients.end())
	{
		if(it->getNickName() == nick)
			return (1);
		++it;
	}
	return (0);
}

void Server::handleNickNameCommand(std::vector<std::string> &params, Client *c)
{
	//handle errors
	std::string err;
	std::cout << "params size for nick " << params.size() << std::endl;
	std::cout << "len of nick value " << params[1].length() << " and its value is " << static_cast<char>(params[1][0]) << std::endl;
	if (!c->getIsAuthenticated())
	{
		err = ERR_NOTREGISTERED(c->getNickName());
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
		return ;
	}
	else if (params.size() < 2 || (params[1] == ":"))
	{
		err = ERR_NONICKNAMEGIVEN(c->getNickName());
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
		return ;
	}
	else if (!isValidNickname(params[1]))
	{
		err = ERR_ERRONEUSNICKNAME(c->getNickName());
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
		return ;
	}
	else if (nickNameAlreadyExists(params[1]))
	{
		err = ERR_NICKNAMEINUSE(c->getNickName());
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
		return ;
	}
	if (!c->getHasNickname())
		c->setHasNickname();
	c->setNickName(params[1]);
}

bool isValidUsername(const std::string& username)
{
    if (username.empty() || username.size() > 9)
        return false;
    for (size_t i = 0; i < username.length(); i++)
	{
        if (!(isalnum(username[i]) || username[i] == '-' || username[i] == '_' || username[i] == '.'))
            return false;
    }
    return true;
}

void Server::handleUserCommand(std::vector<std::string> &params, Client *c)
{
	//handle errors
	//handle long real name
	std::string err;
	std::string username;
	std::string hostname;
	std::string servername;
	std::string realname;

	if (c->getIsAuthenticated())
	{
		err = ERR_ALREADYREGISTERED(c->getNickName());
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
		return ;
	}
	if (params.size() < 5)
	{
		err = ERR_NEEDMOREPARAMS(c->getNickName(), "USER");
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
		return ;
	}
	username = params[1];
	hostname = params[2];
	servername = params[3];
	realname = params[4];
	if (!isValidUsername(username))
	{
		err = ERR_ERRONEUSUSERNAME(username);
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
		return ;
	}
	if (servername != this->_name)
	{
		err = ERR_NOSUCHSERVER(c->getNickName(), servername);
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
		return ;
	}
	if (realname.find(':') != std::string::npos)
	{
		realname = realname.substr(1) + " ";
		for (size_t i = 5; i < params.size(); i++)
		{
			realname += params[i];
			if (i != (params.size() - 1))
				realname += " ";
		}
		std::cout << "realname is " << realname << std::endl;
	}
	c->setUserName(username);
	c->setRealName(realname);
	c->setHostName(hostname);
}

std::string extractMessage(std::string m)
{
	size_t end;

	end = m.find("\r\n");
	if (end != std::string::npos)
		return (m.substr(0, end));
	return (m);
}