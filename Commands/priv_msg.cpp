#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"

void Server::SendPrivMsg_User( std::string &target_name, const std::string &message, Client &c)
{
	if (Server::has_theClient(target_name))
	{
		Client &target_client = Server::get_client(target_name);
		std::string response = ":" + c.getPrefix() + " PRIVMSG " + target_name + " :" + message + "\r\n";
		send(target_client.getClientSocket().getSocketFd(), response.c_str(), response.size(), 0);
	}
	else
	{
		std::string err_ = ERR_NOSUCHNICK(target_name, c.getNickName());
		send(c.getClientSocket().getSocketFd(), err_.c_str(), err_.size(), 0);
	}
}
	

void Server::SendPrivMsg_Channel( std::string &target_channel, const std::string &message, Client &c)
{
	// (void)message;
	if (!has_theChannel(target_channel))
	{
		std::string err = ERR_NOSUCHCHANNEL(c.getNickName(), target_channel);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	Channel &channel = Server::get_channel(target_channel);
	if (!channel.Is_ClientInChannel(c)) // check if the client is in the channel 
	{
		std::string err = ERR_NOTONCHANNEL(c.getNickName(), channel.getChannelName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	// Send the message to all clients in the channel except the sender
	std::string response = ":" + c.getNickName() + " PRIVMSG " + target_channel + " :" + message + "\r\n";
	Server::message_to_Channel(target_channel,response, c );
	// std::vector<Client>::iterator it = channel.getClients().begin();
	// for (; it != channel.getClients().end(); it++)
	// {
	// 	if (it->getNickName() != c.getNickName())
	// 	{
	// 		send(c.getClientSocket().getSocketFd(), response.c_str(), response.size(), 0);
	// 	}
	// }
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
	std::string message ;

	if (params[2][0] == ':')
	{
		for (size_t i = 2; i < params.size(); ++i)
		{
			message += params[i];
			if (i < params.size() - 1)
				message += " ";
		}
		message.erase(0, 1); // Remove the leading ':'
	}
	else
		message = params[2];

	if (target_name[0] == '#')
		Server::SendPrivMsg_Channel(target_name, message, c);
	else
		Server::SendPrivMsg_User(target_name, message, c);
}