#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include "../include/numericReplies.hpp"



bool Server::has_theClient( std::string &nickName) // check if the client exists in the list of clients
{
	std::vector<Client>::iterator it = _allClients.begin();
	for ( ;it != _allClients.end(); it++)
	{
		if (it->getNickName() == nickName)
			return true;
	}
	return false;
}

bool Server::has_theChannel(std::string channelName) // check if the channel exists in the list of channels
{
	std::vector<Channel>::iterator it = _channels.begin();
	for ( ;it != _channels.end(); it++)
	{
		if (it->getChannelName() == channelName)
			return true;
	}
	return false;
}



Channel & Server::get_channel( std::string &channel_name)
{
	std::vector<Channel>::iterator it = _channels.begin();
	for ( ;it != _channels.end(); it++)
	{
		if (it->getChannelName() == channel_name)
			return (*it);
	}
	return _channels[0];
}

Client &Server::get_client( std::string &nickName)
{
	std::vector<Client>::iterator it = _allClients.begin();
	for ( ;it != _allClients.end(); it++)
	{
		if (it->getNickName() == nickName)
			return (*it);
	}
	return _allClients[0];
}


void Server::message_to_Allclients( std::string &channelName, const std::string &message)
{
	Channel &channel = get_channel(channelName);
	std::vector<Client>::iterator it = channel.getClients().begin();
	for (; it != channel.getClients().end(); it++)
		send(it->getClientSocket().getSocketFd(), message.c_str(), message.size(), 0);
}


void Server::message_to_Channel(std::string &channelName, const std::string &message, Client &c)
{
	// Send the message to all clients in the channel except the sender
	Channel &channel = get_channel(channelName);
	std::vector<Client>::iterator it = channel.getClients().begin();
	for (; it != channel.getClients().end(); it++)
	{
		if (it->getNickName() != c.getNickName())
			send(it->getClientSocket().getSocketFd(), message.c_str(), message.size(), 0);
	}
}
void Server::SendChannelInfos(std::string &channelName, Client &c)
{
	std::string listOfClients;

	std::vector<Client> clients = get_channel(channelName).getClients();
	Channel &channel = get_channel(channelName);
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (channel.Is_OperatorInChannel(clients[i]))
			listOfClients += "@" + clients[i].getNickName();
		else
			listOfClients += clients[i].getNickName();
		if (i < clients.size() - 1)
			listOfClients += " ";
		// else
		// 	listOfClients += "\r\n"; // end of the list		
	}
	std::string members_channel = RPL_NAMREPLY(c.getNickName(), channelName, listOfClients);
	send(c.getClientSocket().getSocketFd(), members_channel.c_str(), members_channel.size(), 0);
	if (channel.getTopic() != "")
	{
		std::string TopicMessage = RPL_TOPIC(c.getNickName(), channelName, channel.getTopic());
		send(c.getClientSocket().getSocketFd(), TopicMessage.c_str(), TopicMessage.size(), 0);
	}
	std::string end_of_names = RPL_ENDOFNAMES(c.getNickName(), channelName);
	send(c.getClientSocket().getSocketFd(), end_of_names.c_str(), end_of_names.size(), 0);
}

std::string get_corr_message(std::vector<std::string> &params, int index)
{
	std::string message ;
	if (index < 0 || index >= static_cast<int>(params.size()))
	{
		// std::cerr << "Index out of bounds in get_corr_message" << std::endl;
		return "";
	}
	if (params[index][0] == ':')
	{
		for (size_t i = index; i < params.size(); ++i)
		{
			message += params[i];
			if (i < params.size() - 1)
				message += " ";
		}
		// message.erase(message.begin()); // Remove the leading ':
		message.erase(0, message.find_first_not_of(':'));
		// message = message.substr(1, message.length());
	}
	else
		message = params[index];
	return message;
}

void handleUnkownCommand(std::string cmd, Client *c)
{
    std::string err = ERR_UNKNOWNCOMMAND(cmd);
    if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)
    {
            std::cerr << "failed to send " << err << std::endl;
    }
}

std::string extractMessage(std::string m)
{
    size_t end;
    size_t end_w;
    end = m.find("\r\n");
    if (end != std::string::npos)
            return (m.substr(0, end));
    end_w = m.find("\n");
    if (end_w != std::string::npos)
		return (m.substr(0, end_w));
	return m;
}

// Channel *Server::getChannelByName(const std::string &channelName) // get channel by name from the list of channels
// {
// 	std::vector<Channel>::iterator it = _channels.begin();
// 	for ( ;it != _channels.end(); it++)
// 	{
// 		if (it->getChannelName() == channelName)
// 			return &(*it);
// 	}
// 	return NULL;
// }

// Client *Server::getClientByNickName(std::string nickName) 
// {
// 	std::vector<Client>::iterator it = _allClients.begin();
// 	for (; it != _allClients.end(); it++)
// 	{
// 		if (it->getNickName() == nickName)
// 			return (&(*it));
// 	}
// 	return NULL;
// }

