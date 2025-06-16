#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"

Channel *Server::GetOrCreateChannel(const std::string &channelName)
{
	for (int i = 0; i < _channels.size(); ++i)
	{
		Channel &channel = _channels[i];
		// Check if the channel already exists
		if (channel.getChannelName() == channelName)
			return &channel;
	}
	// If channel does not exist, create a new one
	_channels.push_back(Channel(channelName));
	return &_channels.back();
}

bool Server::valid_joining_channel(Channel *channel, Client *c, const std::string &password)
{

}




void Server::join_each_channel(const std::string &channelName, Client *c, const std::string &password)
{
	if (channelName[0] != '#' || channelName.empty())
	{
		std::string err = ERR_NOSUCHCHANNEL(c->getNickName(), channelName);
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	//create or get the channel
	Channel *channel = GetOrCreateChannel(channelName);
	
	bool isNewChannel = false;
	if (channel->getClientSize() == 0) // if the channel is empty then it is a new channel
		isNewChannel = true;

	if (channel->alreadyInChannel(c))
	{
		std::string err = ERR_USERONCHANNEL(c->getNickName(), c->getNickName(), channelName);
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (valid_joining_channel(channel, c, password))
	{
		// i hve to add the client to the channel

	}
	if (isNewChannel)
		channel->addAsOperator(c);
	else
		channel->addAsClient(c);
	
	// std::string joinMsg = RPL_JOINMSG(c->getHostName(), c->getIpAddress(), channelName);



}

std::vector <std::string> split(const std::string &str, char del)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, del))
		tokens.push_back(token);
	return tokens;
}

void Server::join(std::vector<std::string> &params, Client *c)
{
	if (params.size() < 2)
	{
		std::string err = ERR_NEEDMOREPARAMS(c->getNickName(), "JOIN");
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	std::string channelsNames = params[1];
	std::string channelsPasswords = "";
	if (params.size() > 2)
		channelsPasswords = params[2];
	std::vector<std::string> channels = split(channelsNames, ',');
	std::vector<std::string> passwords = split(channelsPasswords, ',');
	for (size_t i = 0; i < channels.size(); ++i)
		Server::join_each_channel(channels[i], c, passwords[i]);
}
