
#include "../include/Channel.hpp"


	
Channel::Channel(std::string name) : _Channel_name(name), Invite(false), Topic(true), Passwd(false), Limit(0) {}

// Getters
std::string Channel::getChannelName() const 
{return _Channel_name;}
std::vector<Client>& Channel::getClients()  
{return _Clients;}
int Channel::getClientSize() const 
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



// FLAGS SETTER AND GETTERS ==================================

bool	Channel::getterInvite(void)
{
	return(this->Invite);
}
bool	Channel::getterTopic(void)
{
	return(this->Topic);
}
bool	Channel::getterPasswd(void)
{
	return(this->Passwd);
}
int		Channel::getterLimit(void)
{
	return(this->Limit);
}
	
void	Channel::SetterInvite(bool Value_To_Give)
{
	this->Invite = Value_To_Give;
}
void	Channel::SetterTopic(bool Value_To_Give)
{
	this->Topic = Value_To_Give;
}
void	Channel::SetterPasswd(bool Value_To_Give)
{
	this->Passwd = Value_To_Give;
}
void	Channel::SetterLimit(int Value_To_Give)
{
	this->Limit = Value_To_Give;
}







