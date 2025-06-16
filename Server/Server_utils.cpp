#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"

Client *Server::getClientByNickName(std::string nickName) 
{
	std::vector<Client>::iterator it = _allClients.begin();
	for (; it != _allClients.end(); it++)
	{
		if (it->getNickName() == nickName)
			return (&(*it));
	}
	return NULL;
}

Channel *Server::getChannelByName(const std::string &channelName) // get channel by name from the list of channels
{
	for (Channel &channel : _channels) 
	{
		if (channel.getChannelName() == channelName) 
			return &channel;
	}
	return NULL;
}