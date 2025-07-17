#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"

void	Channel::handleModeCommand(std::vector<std::string> &params, Client &c)
{
	int	params_count = params.size() -1;
	int	Flag = 1;
	int	j = 0;
	int current = 1;
	std::string cmd = "MODE";

	while (params[0][j])
	{
		if (params[0][j] == '+')
			Flag = 1;
		else if(params[0][j] == '-')
			Flag = -1;
		else if(params[0][j] == 'i')
		{
			std::string Message;
			if (Flag == 1)
				Message = RPL_CHANGEMODE(c.getNickName(), this->getChannelName(), params[0][j]);
			else if (Flag == -1)
				Message = RPL_CHANGEMODEMINUS(c.getNickName(),this->getChannelName(), params[0][j]);
			message_to_channel2(Message, c);
			this->handleInvite(Flag);
			this->updateCreationTime(); 
		}
		else if(params[0][j] == 't')
		{
			this->handleTopicPermission(Flag);
			if (Flag == 1)
			{
				std::string Message = RPL_CHANGEMODE(c.getNickName(), this->getChannelName(), params[0][j]);
				message_to_channel2(Message, c);
				std::cout << "Setting topic protection for channel: " << this->getChannelName() << std::endl;
	   			 this->updateCreationTime(); 
			}
			else if (Flag == -1)
			{
				std::string Message = RPL_CHANGEMODEMINUS(c.getNickName(), this->getChannelName(), params[0][j]);
				message_to_channel2(Message, c);
				std::cout << "Removing topic protection from channel: " << this->getChannelName() << std::endl;
				this->updateCreationTime();
			}
		}
		else if(params[0][j] == 'k')
		{
			if (handle_Password(Flag, params, current, params_count) == -1)
			{
				std::string err = ERR_NEEDMOREPARAMS(cmd);
				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
				return;
			}
			if (Flag == 1)
			{
				std::string Message = RPL_CHANGEMODE(c.getNickName(), this->getChannelName(), params[0][j]);
				message_to_channel2(Message, c);
				std::cout << "Setting password for channel: " << this->getChannelName() << std::endl;
				this->updateCreationTime();
			}
			else if (Flag == -1)
			{
				std::string Message = RPL_CHANGEMODEMINUS(c.getNickName(), this->getChannelName(), params[0][j]);
				message_to_channel2(Message, c);
				std::cout << "Removing password from channel: " << this->getChannelName() << std::endl;
				this->updateCreationTime();
			}
		}
		else if(params[0][j] == 'l')
		{
			int Ret = handle_Limit(Flag, params, current, params_count);
			if (Ret == -1)
			{
				std::string err = ERR_NEEDMOREPARAMS(cmd);
				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
				return;
			}
			if (Ret == -2)
			{
				std::string err = ERR_INVALIDMODEPARM(c.getUserName(), this->getChannelName(), getChannelMode(), params[0][j]);
				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
				return;
			}
			if (Flag == 1)
			{
				std::string Message = RPL_CHANGEMODE(c.getNickName(), this->getChannelName(), params[0][j]);
				message_to_channel2(Message, c);
				std::cout << "Setting user limit for channel: " << this->getChannelName() << std::endl;
				this->updateCreationTime();
			}
			else if (Flag == -1)
			{
				std::string Message = RPL_CHANGEMODEMINUS(c.getNickName(), this->getChannelName(), params[0][j]);
				message_to_channel2(Message, c);
				std::cout << "Removing user limit from channel: " << this->getChannelName() << std::endl;
				this->updateCreationTime();
			}
		}
		else if(params[0][j] == 'o')
		{
			if (current >= (int)params.size())
			{
				std::string err = ERR_NEEDMOREPARAMS(cmd); 
				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
				return;
			}
			std::string targetNick = params[current++];
			if (!this->Is_ClientInChannel_2(targetNick))
			{
				std::string err = ERR_USERNOTINCHANNEL(targetNick, this->getChannelName());
				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
				return;
			}
			if (Flag == 1)
			{
				if (!this->Is_OperatorInChannel_2(targetNick))
				{
					this->_Operators.push_back(targetNick);
					std::string Message = RPL_UMODEIS(c.getNickName(), this->getChannelName(), params[0][j], targetNick);
					message_to_channel2(Message, c);
		  			this->updateCreationTime();	
				}
				else
				{
					std::string err = ERR_USERONCHANNEL(targetNick, this->getChannelName());
					send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
					return;
				}
			}
			else if (Flag == -1)
			{
				for (std::vector<std::string>::iterator it = this->_Operators.begin(); it != this->_Operators.end(); ++it)
				{
					if (*it == targetNick)
					{
						this->_Operators.erase(it);
						this->updateCreationTime();
						std::string Message = RPL_UMODEISMINUS(c.getNickName(), this->getChannelName(), params[0][j], targetNick);
						message_to_channel2(Message, c);
						break;
					}
				}
			}
		}
		else
		{
			std::string err = ERR_UNKNOWNMODE(c.getNickName(),this->getChannelName(),  params[0][j]);
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		j++;
	}
}

int	Server::initialParsingMode(std::vector<std::string> &params, Client &c)
{
	if (!has_theChannel(params[1]))
	{
		std::string err = ERR_NOSUCHCHANNEL(params[1]);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return (1);
	}
	Channel &channel = get_channel(params[1]);
	if (!channel.Is_ClientInChannel(c))
	{
		std::string err = ERR_NOTONCHANNEL(c.getNickName(), channel.getChannelName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return (1);
	}
	if (!channel.Is_OperatorInChannel(c) && params.size() != 2 )
	{
		std::string err = ERR_CHANOPRIVSNEEDED(channel.getChannelName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return (1);
	}
	return (0);
}

void Server::displayMode(Client &c, Channel channel)
{
	std::ostringstream oss;
	oss << channel.getCreationTime();
	std::string timeStr = oss.str();
	std::string Message;
	Message = RPL_CHANNELMODEIS(c.getNickName(), channel.getChannelName(), channel.getChannelMode());
	send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
	std::string timeMsg = RPL_CREATIONTIME(c.getNickName(), channel.getChannelName(), timeStr);
	send(c.getClientSocket().getSocketFd(), timeMsg.c_str(), timeMsg.size(), 0);
}

void Server::Mode(std::vector<std::string> &params, Client &c)
{
	if (params.size() < 2)
	{
		std::string err = ERR_NEEDMOREPARAMS(params[0]);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return ;
	}
	if (params.size() > 2 &&params[2] == "+sn")
		return;
	if (params[1][0] == '#')
	{
		if (initialParsingMode(params, c) == 1)
			return;
		Channel &channel = get_channel(params[1]);
		if (params.size() == 2)
		{
			displayMode(c, channel);
			return;
		}
		params.erase(params.begin());
		params.erase(params.begin());
		channel.handleModeCommand(params, c);
	}
	else
	{
		std::string err = ERR_NOSUCHCHANNEL(params[1]);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
}
