#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"
#include <ostream>
#include <sys/socket.h>

void Channel::RemoveOperator(std::string &nickname, Client &c)
{
	std::vector<std::string>::iterator it_opp = _Operators.begin();
	for (; it_opp != _Operators.end(); it_opp++)
	{
		if (*it_opp == nickname)
		{
			_Operators.erase(it_opp);
			break;
		}
	}
	std::vector<Client>::iterator it_cl = _Clients.begin();
	for (; it_cl != _Clients.end(); it_cl++)
	{
		if (it_cl->getNickName() == nickname)
		{
			_Clients.erase(it_cl);
			break;
		}
	}
	if (_Operators.empty() && _Clients.size() > 0) //if there only one operator and i kick him, i switch th op to the next client
	{
		Channel::SetterKickFlag(true); // i set the kick flag to false, so that i can kick again
		_Operators.push_back(_Clients[0].getNickName());
		std::string Message = RPL_UMODEIS(c.getNickName(), this->getChannelName(), "o", _Clients[0].getNickName());
		message_to_channel2(Message, c);
	}
}
void Channel::RemoveClient(std::string &nickname)
{
	std::vector<Client>::iterator it_cl = _Clients.begin();
	for (; it_cl != _Clients.end(); it_cl++)
	{
		if (it_cl->getNickName() == nickname)
		{
			_Clients.erase(it_cl);
			break;
		}
	}
}

void Server::kick_by_one(std::string client_to_kick, Client &c, Channel &channel, std::vector<std::string> &params, std::string &ChannelName)
{
	if (!channel.Is_ClientInChannel_2(client_to_kick)) // check if the client that i wanna kick is in the channel
	{
		std::string err = ERR_NOSUCHNICK(c.getNickName() ,client_to_kick);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (channel.Is_OperatorInChannel_2(client_to_kick))
	{
		std::string reason = get_corr_message(params, 3);
		std::string kick_message = RPL_KICK(c.getNickName(), client_to_kick, ChannelName, reason);
		Server::message_to_Allclients(ChannelName, kick_message);
		channel.RemoveOperator(client_to_kick, c);
	}
	if (channel.Is_ClientInChannel_2(client_to_kick))
	{
		std::string reason = get_corr_message(params, 3);
		std::string kick_message = RPL_KICK(c.getNickName(), client_to_kick, ChannelName, reason);
		Server::message_to_Allclients(ChannelName, kick_message);
		channel.RemoveClient(client_to_kick);
	}
}

void Server::kick_from_channel(std::vector<std::string> &params, Client &c)
{
	if (params.size() < 2 || (params[1].empty() && params[2].empty() && params[3] == ":" ))
	{
		std::string err = ERR_NEEDMOREPARAMS(params[0]);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	std::string ChannelName ;
	ChannelName = params[1];
	if (ChannelName[0] != '#' || ChannelName.empty() || !Server::has_theChannel(ChannelName))
	{
		std::string err = ERR_NOSUCHCHANNEL(ChannelName);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (params.size() == 2)
	{
		std::string err = ERR_NEEDMOREPARAMS(params[0]);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}

	Channel &channel = get_channel(ChannelName);
	if (!channel.Is_ClientInChannel(c)) // check if the client is in the channel
	{
		std::string err = ERR_NOTONCHANNEL(c.getNickName(), ChannelName);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}

	if (!channel.Is_OperatorInChannel(c))
	{
		std::string opp_error = ERR_CHANOPRIVSNEEDED(ChannelName);
		send(c.getClientSocket().getSocketFd(), opp_error.c_str(), opp_error.size(), 0);
		return ;
	}
	std::string clients_to_kick = params[2];
	std::vector<std::string> clients_need_ToKick  = splitBy_delimeter(clients_to_kick, ',');
	for (size_t i = 0; i < clients_need_ToKick.size(); i++)
	{
		Server::kick_by_one(clients_need_ToKick[i], c, channel, params, ChannelName);
		if (channel.getterKickFlag() == true)
		{
			channel.SetterKickFlag(false);
			break;
		}
	}
}
