#include "../include/Authentification.hpp"
#include "../include/numericReplies.hpp"

void sendError(std::string err, Client *c)
{
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		{
			std::cerr << "failed to send " << err << std::endl;
		}
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

void Server::handlePassCommand(std::vector<std::string> &params, Client *c)
{
	//handle errors
	std::string err;
	if (c->getIsRegistered())
	{
        sendError(ERR_ALREADYREGISTERED(c->getNickName()), c);
		// err = ERR_ALREADYREGISTERED(c->getNickName());
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
	}
	else if (params.size() < 2)
	{
        sendError(ERR_NEEDMOREPARAMS(params[0]), c);
		// err = ERR_NEEDMOREPARAMS(params[0]);
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
	}
	else if (params[1] != _password)
	{
        sendError(ERR_PASSWDMISMATCH(c->getNickName()), c);
		// err = ERR_PASSWDMISMATCH(c->getNickName());
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
	}
	else
		c->setIsRegistered(true);
}

void Server::handleNickNameCommand(std::vector<std::string> &params, Client *c)
{
	//handle errors
	std::string err;
	if (!c->getIsRegistered())
	{
        sendError(ERR_NOTREGISTERED(c->getNickName()), c);
		// err = ERR_NOTREGISTERED(c->getNickName());
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
		return ;
	}
	else if (params.size() < 2 || (params[1] == ":"))
	{
        sendError(ERR_NONICKNAMEGIVEN(c->getNickName()), c);
		// err = ERR_NONICKNAMEGIVEN(c->getNickName());
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
		return ;
	}
	else if (!isValidNickname(params[1]))
	{
        sendError(ERR_ERRONEUSNICKNAME(c->getNickName()), c);
		// err = ERR_ERRONEUSNICKNAME(c->getNickName());
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
		return ;
	}
	else if (nickNameAlreadyExists(params[1]))
	{
        sendError(ERR_NICKNAMEINUSE(c->getNickName()), c);
		// err = ERR_NICKNAMEINUSE(c->getNickName());
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
		return ;
	}
	if (!c->getHasNickname())
		c->setHasNickname();
	c->setNickName(params[1]);
	if (c->getHasNickname() && c->getHasUser())
	{
        sendError(RPL_WELCOME(c->getNickName(), c->getHostName()), c);
		// err = RPL_WELCOME(c->getNickName(), c->getHostName());
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
		c->setIsAuthenticated(true);
	}
}

void Server::handleUserCommand(std::vector<std::string> &params, Client *c)
{
	std::string err;
	std::string username;
	std::string hostname;
	std::string servername;
	std::string realname;

	if (!c->getIsRegistered())
	{
        sendError(ERR_NOTREGISTERED(c->getNickName()), c);
		// err = ERR_NOTREGISTERED(c->getNickName());
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
		// return ;
	}
	if (c->getIsAuthenticated())
	{
        sendError(ERR_ALREADYREGISTERED(c->getNickName()), c);
		// err = ERR_ALREADYREGISTERED(c->getNickName());
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
		return ;
	}
	if (params.size() < 5)
	{
		err = ERR_NEEDMOREPARAMS(params[0]);
        sendError(ERR_NEEDMOREPARAMS(params[0]), c);
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
		return ;
	}
	username = params[1];
	hostname = params[2];
	servername = params[3];
	realname = params[4];
	//!TODO: need to check if username is valid!!! 
	// if (!isValidUsername(username))
	// {
	// 	err = ERR_ERRONEUSUSERNAME(username);
	// 	if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
	// 	{
	// 		std::cerr << "failed to send " << err << std::endl;
	// 	}
	// 	return ;
	// }
	if (realname.find(':') != std::string::npos)
	{
		realname = realname.substr(1) + " ";
		for (size_t i = 5; i < params.size(); i++)
		{
			realname += params[i];
			if (i != (params.size() - 1))
				realname += " ";
		}
	}
	c->setUserName(username);
	c->setRealName(realname);
	c->setHostName(hostname);
	c->setHasUserame();
	if(c->getHasNickname() && c->getHasUser())
	{
		// err = RPL_WELCOME(c->getNickName(), c->getHostName());
        sendError(RPL_WELCOME(c->getNickName(), c->getHostName()), c);
		// if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
		// {
		// 	std::cerr << "failed to send " << err << std::endl;
		// }
		c->setIsAuthenticated(true);
	}
}
