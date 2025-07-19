#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"

void Server::SendPrivMsg_User(std::string &target_name, const std::string &message, Client &c)
{
	if (Server::has_theClient(target_name))
	{
		Client &target_client = Server::get_client(target_name);
		std::string response = RPL_PRIVMSG(c.getNickName(), c.getUserName(), c.getClientSocket().getIpAddress(), target_name, message);
		send(target_client.getClientSocket().getSocketFd(), response.c_str(), response.size(), 0);
	}
	else
	{
		std::string err_ = ERR_NOSUCHNICK(c.getNickName(), target_name);
		send(c.getClientSocket().getSocketFd(), err_.c_str(), err_.size(), 0);
	}
}
	

void Server::SendPrivMsg_Channel( std::string &target_channel, const std::string &message, Client &c)
{
	if (!has_theChannel(target_channel))
	{
		std::string err = ERR_NOSUCHCHANNEL(target_channel);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	Channel &channel = Server::get_channel(target_channel);
	if (!channel.Is_ClientInChannel(c))
	{
		std::string err = ERR_NOTONCHANNEL(c.getNickName(), channel.getChannelName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	std::string response = RPL_PRIVMSG(c.getNickName(), c.getUserName(), c.getClientSocket().getIpAddress(), target_channel, message);
	Server::message_to_Channel(target_channel,response, c );
}

void Server::priv_msg(std::vector<std::string> &params, Client &c)
{
	if (params.size() == 1)
	{
		std::string err = ERR_NORECIPIENT(c.getNickName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	else if (params.size() == 2)
	{
		std::string err = ERR_NOTEXTTOSEND(c.getNickName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}

	std::string target_name = params[1];
	std::string message = get_corr_message(params, 2);
	if (target_name[0] == '#')
		Server::SendPrivMsg_Channel(target_name, message, c);
	else
		Server::SendPrivMsg_User(target_name, message, c);
}