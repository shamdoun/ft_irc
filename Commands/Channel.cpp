#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"

Channel::Channel(std::string name) : _Channel_name(name) {}

// Getters
std::string Channel::getChannelName() const{return _Channel_name;}
std::vector<Client*> Channel::getClients() const {return _Clients;}
int Channel::getClientSize() const {	return _Clients.size();}

bool Channel::alreadyInChannel(Client *client) const 
{
	for (const Client* client_channel : _Clients) 
	{
		if (client_channel->getNickName() == client->getNickName()) 
			return true;
	}
	return false;
}

void Channel::addAsOperator(Client *client) 
{
	if (client && !alreadyInChannel(client)) 
	{
		_Clients.push_back(client);
		_Operators.push_back(client->getNickName());
	}	
}

void Channel::addAsClient(Client *client) 
{
	if (client && !alreadyInChannel(client)) 
	{
		_Clients.push_back(client);
	}
}

