
#include "../include/Channel.hpp"
#include <_ctype.h>


	
Channel::Channel(std::string name) : _Channel_name(name), Invite(false), Topic(false), Passwd(false), pass_as_stirng(""), Limit(0), Client_who_updated(""), stop_kick(false)
{
	TimeOfCreatiion = time(NULL);
	Topic_Update_Time = time(NULL);
}

std::string Channel::getChannelName() const 
{return _Channel_name;}
std::vector<Client>& Channel::getClients()  
{return _Clients;}
std::vector<Client>& Channel::getOpperators()
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
		if (it->getId() == client.getId()) 
			return true;
	}
	return false;
}

bool Channel::Is_ClientInChannel_2(size_t id)  
{
	std::vector<Client>::iterator it = _Clients.begin();
	for (; it != _Clients.end(); it++)
	{
		if (it->getId() == id) 
			return true;
	}
	return false;
}
bool Channel::Is_OperatorInChannel(Client &client)  
{
	std::vector<Client>::iterator it = _Operators.begin();
	for (; it != _Operators.end(); it++)
	{
		if (it->getId() == client.getId()) 
			return true;
	}
	return false;
}

bool Channel::Is_OperatorInChannel_2(size_t id)  
{
	std::vector<Client>::iterator it = _Operators.begin();
	for (; it != _Operators.end(); it++)
	{
		if (it->getId() == id) 
			return true;
	}
	return false;
}



void Channel::addAsOperator(Client &client) 
{
	if (!Is_ClientInChannel(client))
	{
		_Clients.push_back(client);
		_Operators.push_back(client);
	}	
}

void Channel::addAsClient(Client &client) 
{
	if (!Is_ClientInChannel(client))
	{
		_Clients.push_back(client);
	}
}


bool	Channel::getterInvite(void)
{
	return(this->Invite);
}

bool Channel::getterKickFlag()
{
	return(this->stop_kick);
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
void	Channel::SetterKickFlag(bool Value_To_Give)
{
	this->stop_kick = Value_To_Give;
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
		this->Passwd = false;
		this->pass_as_stirng = "";
		return (0);
	}
	else
	{
		if (current > prms_count)
		{
			return (-1);
		}
		else
		{
			this->Passwd = true;
			this->pass_as_stirng = params[current];
			current++;
			return (0);
		}
	}	
}

bool Channel::isAllDigits(const std::string& str)
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
		if (current > prms_count)
		{
			return (-1);
		}
		else if (!this->isAllDigits(params[current]))
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

void	Channel::message_to_channel2(std::string &message, Client &c)
{
	(void)c;
	std::vector<Client>::iterator it = _Clients.begin();
	for (; it != _Clients.end(); it++)
		send(it->getClientSocket().getSocketFd(), message.c_str(), message.size(), 0);
}

std::string Channel::getChannelMode(Server &server)
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
			mode += " @" + server.getNickNameById(this->_Operators[i].getId());
			if (i < this->_Operators.size() - 1)
				mode += " ";
		}
	}
	if (mode != "")
	{
		mode = "+" + mode;
	}
	
	return (mode);
}

time_t Channel::getCreationTime() 
{
	return (TimeOfCreatiion);
}

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

int	Channel::addAsInvited(Client &client) 
{
	if (!Is_ClientInChannel(client) && !Is_Invited(client))
	{
		_Invited.push_back(client);
		return (1);
	}
	return (-1);
}

int Channel::Is_Invited(Client &client) 
{
	for (std::vector<Client>::iterator it = _Invited.begin(); it != _Invited.end(); it++)
	{
		if (it->getId() == client.getId()) 
			return 1;
	}
	return 0;
}


void Channel::RemoveFromInvite(Client &client)
{
	std::vector<Client>::iterator it_cl = this->_Invited.begin();
	for (; it_cl != this->_Invited.end(); it_cl++)
	{
		if (it_cl->getId() == client.getId())
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