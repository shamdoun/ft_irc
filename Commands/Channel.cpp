
#include "../include/Channel.hpp"


	
Channel::Channel(std::string name) : _Channel_name(name), Invite(false), Topic(true), Passwd(false), Limit(0) {}

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

void	Channel::PrintChannelInfo(void)
{
	std::cout << "Channel Name: " << this->_Channel_name << std::endl;
	std::cout << "Invite: " << (this->Invite ? "true" : "false") << std::endl;
	std::cout << "Topic: " << (this->Topic ? "true" : "false") << std::endl;
	std::cout << "Password: " << (this->Passwd ? "true" : "false") << std::endl;
	std::cout << "Limit: " << this->Limit << std::endl;
	std::cout << "Number of Clients in Channel: " << this->getClientSize_inChannel() << std::endl;
	for (size_t i = 0; i < _Clients.size(); i++)
	{
		std::cout << "Client " << i + 1 << ": " << _Clients[i].getNickName() << std::endl;
	}
	std::cout << "Operators: ";
	for (size_t i = 0; i < _Operators.size(); i++)
	{
		std::cout << _Operators[i];
		if (i < _Operators.size() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;
	std::cout << "------------------------" << std::endl;
}






