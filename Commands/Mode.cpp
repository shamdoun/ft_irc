#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"


void	Channel::handleModeCommand(std::vector<std::string> &params, Client *c)
{
	std::string debug;
	
	//printing parrams for debugging
	// for (size_t i = 0; i < params.size(); i++)
	// {
	// 	debug += params[i] + " || ";
	// }
	// debug += "\n";
	send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);

	if (params.size() < 2)  // params
	{
		std::string err = ERR_NEEDMOREPARAMS(c->getNickName(), "MODE");
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (params[1][0] == '+')
	{

		debug = "Adding modes: ";
		send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		if (params[1].find('i') != std::string::npos)
		{
			this->SetterInvite(true);
			debug = "Invite mode set to true\n";
			send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		}
		if (params[1].find('t') != std::string::npos)
		{
			this->SetterTopic(true);
			debug = "Topic mode set to true\n";
			send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		}
		if (params[1].find('k') != std::string::npos)
		{
			this->SetterPasswd(true);
			this->SetterPassAsString(params[2]);
			debug = "Password mode set to true\n" + params[2];
			send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		}
		if (params[1].find('l') != std::string::npos)
		{
			this->SetterLimit(atoi(params[2].c_str()));
			// debug = "Limit mode set to " + std::to_string(this->getterLimit());
			send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		
		}
	}
	else if (params[1][0] == '-')
	{
		debug = "Removing modes: ";
		send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		if (params[1].find('i') != std::string::npos)
		{
			this->SetterInvite(false);
			debug = "Invite mode set to false";
			send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		}
		if (params[1].find('t') != std::string::npos)
		{
			this->SetterTopic(false);
			debug = "Topic mode set to false";
			send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		}
		if (params[1].find('k') != std::string::npos)
		{
			this->SetterPasswd(false);
			this->SetterPassAsString("");
			debug = "Password mode set to false";
			send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		}
		if (params[1].find('l') != std::string::npos)
		{
			this->SetterLimit(0);
			debug = "Limit mode set to 0 (no limit)";
			send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		}
	}
	else
	{
		debug = "Unknown mode: " + params[1];
		send(c->getClientSocket().getSocketFd(), debug.c_str(), debug.size(), 0);
		std::string err = ERR_UNKNOWNMODE(c->getNickName(), params[1]);
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	// std::string modeReply = RPL_CHANNELMODEIS(c->getNickName(), this->_Channel_name, params[1]);
	// send(c->getClientSocket().getSocketFd(), modeReply.c_str(), modeReply.size(), 0);
}


void Server::Mode(std::vector<std::string> &params, Client &c)
{
	if (params.size() < 2)  // params
	{
		std::string err = ERR_NEEDMOREPARAMS(c.getNickName(), "MODE");
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (params[1][0] == '#') //incorrect channel Name
	{
		if (!has_theChannel(params[1]))  //incorrect channel
		{
			std::string err = ERR_NOSUCHCHANNEL(c.getNickName(), params[1]);
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		Channel &channel = get_channel(params[1]);
		if (!channel.Is_ClientInChannel(c)) // Client is not in channel
		{
			std::string err = ERR_NOTONCHANNEL(c.getNickName(), channel.getChannelName());
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		if (!channel.Is_OperatorInChannel(c)) // Client is an operator in channel
		{
			std::string err = ERR_CHANOPRIVSNEEDED(c.getNickName(), channel.getChannelName());
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		else
		{
			params.erase(params.begin()); // remove the command MODE
			channel.handleModeCommand(params, &c); // handle the mode command
			channel.PrintChannelInfo();
		}

	}
	else
	{
		std::string err = ERR_NOSUCHCHANNEL(c.getNickName(), "MODE");   /// CHANGE ERROR !
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}

}