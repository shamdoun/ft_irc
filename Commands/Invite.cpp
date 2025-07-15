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
	std::string channelName = params[1];
	std::string nickName = params[2];
	//if channel does not exist check 
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
	std::string Target_NickName = targetClient.getNickName();
	channel.addAsInvited(Target_NickName);
	std::string message = RPL_INVITING(c.getNickName(), channel.getChannelName(), Target_NickName);
	send(targetClient.getClientSocket().getSocketFd(), message.c_str(), message.size(), 0);
	std::string inviteMessage = RPL_INVITE(c.getNickName(), channel.getChannelName(), Target_NickName);
	send(c.getClientSocket().getSocketFd(), inviteMessage.c_str(), inviteMessage.size(), 0);
	std::cout << "Client " << c.getNickName() << " invited " << Target_NickName << " to channel " << channel.getChannelName() << std::endl;
}
