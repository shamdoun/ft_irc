
#include "../include/Channel.hpp"
#include <_ctype.h>


	
Channel::Channel(std::string name) : _Channel_name(name), Invite(false), Topic(false), Passwd(false), pass_as_stirng(""), Limit(0), Client_who_updated("")
{
	TimeOfCreatiion = time(NULL);
	Topic_Update_Time = time(NULL);
}

// Getters
std::string Channel::getChannelName() const 
{return _Channel_name;}
std::vector<Client>& Channel::getClients()  
{return _Clients;}
std::vector<std::string>& Channel::getOpperators()
{
	return _Operators;
}
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

bool Channel::Is_ClientInChannel_2(std::string NickName)  
{
	std::vector<Client>::iterator it = _Clients.begin();
	for (; it != _Clients.end(); it++)
	{
		if (it->getNickName() == NickName) 
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

bool Channel::Is_OperatorInChannel_2(std::string &nickName)  
{
	std::vector<std::string>::iterator it = _Operators.begin();
	for (; it != _Operators.end(); it++)
	{
		if (*it == nickName) 
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

std::string &Channel::getterPassAsString(void)
{
	return (this->pass_as_stirng);
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

void	Channel::SetterPassAsString(std::string Value_To_Give)
{
	 this->pass_as_stirng = Value_To_Give;
}


void	Channel::handleInvite(int flag)
{
	if (flag == 1)
	{
		this->Invite = true;
	}
	if (flag == -1)
		this->Invite = false;
}

void	Channel::handleTopicPermission(int flag)
{
	if (flag == 1)
		this->Topic = true;
	if (flag == -1)
		this->Topic = false;
}

int		Channel::handle_Password(int flag, std::vector<std::string> &params, int	&current, int prms_count)
{
	if (flag == -1)
	{
		// std::cout << "handle password with negative " << std::endl;
		this->Passwd = false;
		this->pass_as_stirng = "";
		return (0);
	}
	else
	{
		// std::cout << "handle password with positive " << std::endl;
		if (current > prms_count)
		{
			// std::cout << "Not enough params" << std::endl;
			return (-1);
		}
		else
		{
			// std::cout << "Succes" << std::endl;
			this->Passwd = true;
			this->pass_as_stirng = params[current];
			current++;
			return (0);
		}
	}	
}

int ft_IsAllnum(char *str)
{
	for (int i = 0; str[i]; i++) {
		if (str[i] < '0' || str[i] > '9')
		{
			return (-1);
		}
	}
	return (0);
}

bool isAllDigits(const std::string& str)
{
	size_t i = 0;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int		Channel::handle_Limit(int flag, std::vector<std::string> &params, int	&current, int prms_count)
{
	if (flag == -1)
	{
		this->Limit = 0;
		return (0);
	}
	else
	{
		// std::cout << "handle password with positive " << std::endl;
		if (current > prms_count)
		{
			return (-1);
		}
		else if (!isAllDigits(params[current]))
		{
			return (-2);
		}
		else
		{
			this->Limit = atoi(params[current].c_str());
			current++;
			return (0);
		}
	}	
}



void	Channel::PrintChannelInfo(void)
{
	std::cout << "Channel Name: " << this->_Channel_name << std::endl;
	std::cout << "Invite: " << (this->Invite ? "true" : "false") << std::endl;
	std::cout << "Topic: " << (this->Topic ? "true" : "false") << std::endl;
	std::cout << "Password: " << (this->Passwd ? "true" : "false") << std::endl;
	std::cout << "Password as string " << this->pass_as_stirng << std::endl;
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



// void Server::message_to_Channel(std::string &channelName, const std::string &message, Client &c)
// {
// 	// Send the message to all clients in the channel except the sender
// 	Channel &channel = get_channel(channelName);
// 	std::vector<Client>::iterator it = channel.getClients().begin();
// 	for (; it != channel.getClients().end(); it++)
// 	{
// 		if (it->getNickName() != c.getNickName())
// 			send(it->getClientSocket().getSocketFd(), message.c_str(), message.size(), 0);
// 	}
// }

void	Channel::message_to_channel2(std::string &message, Client &c)
{
	// Send the message to all clients in the channel except the sender
	(void)c;
	std::vector<Client>::iterator it = _Clients.begin();
	for (; it != _Clients.end(); it++)
	{
		// if (it->getNickName() != c.getNickName())
			send(it->getClientSocket().getSocketFd(), message.c_str(), message.size(), 0);
	}
}

std::string Channel::getChannelMode(void)
{
	std::string mode = "";
	if (this->Invite)
		mode += "i";
	if (this->Topic)
		mode += "t";
	if (this->Passwd)
		mode += "k";
	if (this->Limit > 0)
		mode += "l";
	if (this->_Operators.size() > 0)
	{
		mode += "o";
		for (size_t i = 0; i < this->_Operators.size(); i++)
		{
			mode += " @" + this->_Operators[i];
			if (i < this->_Operators.size() - 1)
				mode += " ";
		}
	}
	if (mode != "")
	{
		mode = "+" + mode; // Add '+' at the beginning if there are modes set
	}
	
	return (mode);
}

time_t Channel::getCreationTime() 
{
	return (TimeOfCreatiion);
}
//update the time of creation
void Channel::updateCreationTime() 
{
  TimeOfCreatiion = time(NULL);
}

void Channel::TopicUpdateTime()
{
	Topic_Update_Time = time(NULL);
}

time_t Channel::getTopicUpdateTime() 
{
	return (Topic_Update_Time);
}

void 	Channel::addAsInvited(std::string &nickName)
{
	if (!Is_ClientInChannel_2(nickName))
	{
		_Invited.push_back(nickName);
		std::cout << "Added " << nickName << " to invited list of channel: " << this->getChannelName() << std::endl;
	}
	else
	{
		std::cout << nickName << " is already in the channel: " << this->getChannelName() << std::endl;
	}
}

int Channel::Is_Invited(std::string &nickName)
{
	std::vector<std::string>::iterator it = _Invited.begin();
	for (; it != _Invited.end(); it++)
	{
		if (*it == nickName) 
			return 1;
	}
	return 0;
}

void Channel::RemoveFromInvite(std::string &nickName)
{
	std::vector<std::string>::iterator it_cl = this->_Invited.begin();
	for (; it_cl != this->_Invited.end(); it_cl++)
	{
		if (*it_cl == nickName)
		{
			this->_Invited.erase(it_cl);
			break;
		}
	}
}



std::string Channel::getTopic(void) const
{
	return (this->_topic);
}

void	Channel::SetterTopicAsString(std::string Value_To_Give)
{
	this->_topic = Value_To_Give;
}

std::string Channel::WhoUpdatedGetter(void)
{
	return Client_who_updated;
}

void Channel::WhoUpdatedSetter(std::string WhoChandedstr)
{
	this->Client_who_updated = WhoChandedstr;
}