#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"

void	Server::Invite_Command(std::vector<std::string> &params, Client &c)
{
	if (params.size() < 3)
	{
		std::string err = ERR_NEEDMOREPARAMS(params[0]);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	std::string channelName = params[2];
	std::string nickName = params[1];
	if (!has_theChannel(channelName))
	{
		std::string err = ERR_NOSUCHCHANNEL(channelName);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	Channel &channel = get_channel(channelName);
	if (!channel.Is_ClientInChannel(c))
	{
		std::string err = ERR_NOTONCHANNEL(c.getNickName(), channel.getChannelName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (!channel.Is_OperatorInChannel(c))
	{
		std::string err = ERR_CHANOPRIVSNEEDED(channel.getChannelName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (!has_theClient(nickName))
	{
		std::string err = ERR_NOSUCHNICK( c.getNickName(), nickName);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	Client &targetClient = get_client(nickName);
	if (channel.Is_ClientInChannel(targetClient))
	{
		std::string err = ERR_USERONCHANNEL(channel.getChannelName(), targetClient.getNickName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	channel.addAsInvited(targetClient);
	std::string message = RPL_INVITING(c.getNickName(), channel.getChannelName(), targetClient.getNickName());
	send(c.getClientSocket().getSocketFd(), message.c_str(), message.size(), 0);
	std::string message2 = MSG_INVITE(c.getNickName(), c.getUserName(), targetClient.getNickName(), channel.getChannelName());
	send(targetClient.getClientSocket().getSocketFd(), message2.c_str(), message2.size(), 0);
}
