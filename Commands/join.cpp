#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"

Channel *Server::GetOrCreateChannel(const std::string &channelName)
{
	for (Channel &channel : _channels)
	{
		if (channel.getChannelName() == channelName)
		{
			return &channel;
		}
	}
	// If channel does not exist, create a new one
	_channels.push_back(Channel(channelName));
	return &_channels.back();
}

void Server::join(std::vector<std::string> &params, Client *c)
{
	if (params.size() < 2)
	{
		std::string err = ERR_NEEDMOREPARAMS(c->getNickName(), "JOIN");
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	std::string Channel_name = params[1];
	// std::string Channel_password = "";
	// if (params.size() >= 3)
	// 	Channel_password = params[2];

	if (Channel_name[0] != '#')
	{
		std::string err = ERR_NOSUCHCHANNEL(c->getNickName(), Channel_name);
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	// check if channel already exists
	Channel *channel = GetOrCreateChannel(Channel_name);


}
