#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"

void Server::SendPrivMsg_User(const std::string &target_name, const std::string &message, Client *c)
{
	Client *target_client = Server::getClientByNickName (target_name);
	if (target_client)
	{
		std::string response = ":" + c->getNickName() + " PRIVMSG " + target_name + " :" + message + "\r\n";
		send(target_client->getClientSocket().getSocketFd(), response.c_str(), response.size(), 0);
	}
	else
	{
		std::string err_ = ERR_NOSUCHNICK(target_name, c->getNickName());
		send(c->getClientSocket().getSocketFd(), err_.c_str(), err_.size(), 0);
	}
}
	

void Server::SendPrivMsg_Channel(const std::string &target_name, const std::string &message, Client *c)
{
	Channel *channel = Server::getChannelByName(target_name);
	if (!channel)
	{
		std::string err = ERR_NOSUCHCHANNEL(c->getNickName(), target_name);
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (!channel->alreadyInChannel(c))
	{
		std::string err = ERR_NOTONCHANNEL(c->getNickName(), channel->getChannelName());
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	// Send the message to all clients in the channel except the sender
	for (Client *client : channel->getClients())
	{
		if (client != c) // Don't send the message to the sender
		{
			std::string response = ":" + c->getNickName() + " PRIVMSG " + target_name + " :" + message + "\r\n";
			send(client->getClientSocket().getSocketFd(), response.c_str(), response.size(), 0);
		}
	}

}

void Server::priv_msg(std::vector<std::string> &params, Client *c)
{
	if (params.size() == 2)
	{
		std::string err = ERR_NORECIPIENT(c->getNickName());
		send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	else if (params.size() < 3)
	{
		std::string err = ERR_NOTEXTTOSEND(c->getNickName());
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
		SendPrivMsg_Channel(target_name, message, c);
	else
		Server::SendPrivMsg_User(target_name, message, c);
}