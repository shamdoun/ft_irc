
#include "../include/Channel.hpp"

Channel::Channel(std::string name) : _Channel_name(name) {}

// Getters
std::string Channel::getChannelName() const 
{return _Channel_name;}
std::vector<Client>& Channel::getClients()  
{return _Clients;}
int Channel::getClientSize_inChannel() const 
{	return _Clients.size();}

bool Channel::Is_ClientInChannel(Client &client)  
{
	std::vector<Client>::iterator it = _Clients.begin();
	for (; it != _Clients.end(); it++)
	{
		if (it->getNickName() == client.getNickName()) 
			return true;
	}
	return false;
}

bool Channel::Is_OperatorInChannel(Client &client)  
{
	std::vector<std::string>::iterator it = _Operators.begin();
	for (; it != _Operators.end(); it++)
	{
		if (*it == client.getNickName()) 
			return true;
	}
	return false;
}


void Channel::addAsOperator(Client &client) 
{
	if (!Is_ClientInChannel(client))
	{
		_Clients.push_back(client);
		_Operators.push_back(client.getNickName());
	}	
}

void Channel::addAsClient(Client &client) 
{
	if (!Is_ClientInChannel(client))
	{
		_Clients.push_back(client);
	}
}

