#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"

Channel &Server::GetOrCreateChannel(const std::string &channelName)
{
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		// Check if the channel already exists
		if (_channels[i].getChannelName() == channelName)
			return _channels[i];
	}
	// If channel does not exist, create a new one
	_channels.push_back(Channel(channelName));
	return _channels.back();
}

// bool Server::valid_joining_channel(Channel &channel, Client &c, const std::string &password)
// {
// 	if (channel.getterLimit() >  channel.getClientSize_inChannel())
// 	{
// 		std::string err_join = ERR_CHANNELISFULL(c.getNickName(), channel.getChannelName());
// 		send(c.getClientSocket().getSocketFd(), err_join.c_str(), err_join.size(), 0);
// 		return false;
// 	}
// 	if (channel.getterInvite())
// 	{
// 		std::string err_join = ERR_INVITEONLYCHAN(c.getNickName(), channel.getChannelName());
// 		send(c.getClientSocket().getSocketFd(), err_join.c_str(), err_join.size(), 0);
// 		return false;
// 	}
// 	if (channel.getterPasswd())
// 	{
// 		if (!channel.getterPassAsString().empty())
// 		{
// 			if(channel.getterPassAsString().compare(password))
// 			{
				
// 				return false;
// 			}
// 			else
// 			{
				
// 			}
// 		}
// 	}
// }




void Server::join_each_channel(std::string &channelName, Client &c, const std::string &password)
{
	(void)password; // password is not used in this implementation, but can be used for future enhancements
	if (channelName[0] != '#' || channelName.empty())
	{
		std::string err = ERR_NOSUCHCHANNEL(c.getNickName(), channelName);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	//create or get the channel
	Channel &channel = GetOrCreateChannel(channelName);
	std::string Pass_check;
	bool isNewChannel = false;
	if (channel.getClientSize_inChannel() == 0) // if the channel is empty then it is a new channel
		isNewChannel = true;

	if (channel.Is_ClientInChannel(c))
	{
		std::string err = ERR_USERONCHANNEL(c.getNickName(), c.getNickName(), channelName);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	// if (valid_joining_channel(channel, c, password))
	// {
	// 	// i hve to add the client to the channel

	// }
	if (isNewChannel)
		channel.addAsOperator(c);
	else
		channel.addAsClient(c);
	std::string newchannelName = channelName.substr(1, channelName.length());
	std::string joinMsg = RPL_JOINMSG(c.getAlteredHost(), c.getClientSocket().getIpAddress(), newchannelName);
	send(c.getClientSocket().getSocketFd(), joinMsg.c_str(), joinMsg.size(), 0); // send join message to the client who has joined

	Server::message_to_Channel(newchannelName, joinMsg, c); // send join message to all clients in the channel except onewho has joined
	Server::SendChannelInfos(newchannelName, c);
}

std::vector <std::string> splitBy_delimeter(const std::string &str, char del)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, del))
		tokens.push_back(token);
	return tokens;
}

void Server::join(std::vector<std::string> &params, Client &c)
{
	if (params.size() < 2)
	{
		std::string err = ERR_NEEDMOREPARAMS(c.getNickName(), "JOIN");
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	std::string channelsNames = params[1];
	std::string channelsPasswords = "";
	if (params.size() > 2)
		channelsPasswords = params[2];
	std::vector<std::string> channels = splitBy_delimeter(channelsNames, ',');
	std::vector<std::string> passwords = splitBy_delimeter(channelsPasswords, ',');
	for (size_t i = 0; i < channels.size(); ++i)
		Server::join_each_channel(channels[i], c, passwords[i]);
}
