#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"

Channel::Channel(std::string name) : _name(name) {}

// Getters
std::string Channel::getChannelName() const{return _name;}
std::vector<Client*> Channel::getClients() const {return _Clients;}
int Channel::getClientSize() const {	return _Clients.size();}

bool Channel::is_member(Client *client) const 
{
	for (const Client* c : _Clients) 
	{
		if (c->getNickName() == client->getNickName()) 
			return true;
	}
	return false;
}