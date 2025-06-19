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


// void Server::message_to_Allclients( std::string &channelName, const std::string &message, Client &c)
// {
// 	Channel &channel = get_channel(channelName);
// 	std::vector<Client>::iterator it = channel.getClients().begin();
// 	for (; it != channel.getClients().end(); it++)
// 	{
// 		std::string joinMsg = RPL_JOINMSG(c.getAlteredHost(), c.getClientSocket().getIpAddress(), channelName);
// 		send(it->getClientSocket().getSocketFd(), joinMsg.c_str(), joinMsg.size(), 0);
// 	}
// }


void Server::message_to_Channel( std::string &channelName, const std::string &message, Client &c)
{
	// Send the message to all clients in the channel except the sender
	Channel &channel = get_channel(channelName);
	std::vector<Client>::iterator it = channel.getClients().begin();
	for (; it != channel.getClients().end(); it++)
	{
		if (it->getNickName() != c.getNickName())
		{
			// std::string response = ":" + c.getNickName() + " PRIVMSG " + channelName + " :" + message + "\r\n";
			send(it->getClientSocket().getSocketFd(), message.c_str(), message.size(), 0);
		}
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
	std::string end_of_names = RPL_ENDOFNAMES(c.getNickName(), channelName);
	send(c.getClientSocket().getSocketFd(), end_of_names.c_str(), end_of_names.size(), 0);
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

