#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"


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
	}
	else
	{
		std::string err = ERR_NOSUCHCHANNEL(c.getNickName(), "MODE");   /// CHANGE ERROR !
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
}