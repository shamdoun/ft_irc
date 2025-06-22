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

	std::cout << "Params_count ===> " << params_count << std::endl;
	while (params[0][j])
	{
		if (params[0][j] == '+')
			Flag = 1;
		else if(params[0][j] == '-')
			Flag = -1;
		else if(params[0][j] == 'i')
			this->handleInvite(Flag);
		else if(params[0][j] == 't')
			this->handleTopicPermission(Flag);
		else if(params[0][j] == 'k')
		{
			if (handle_Password(Flag, params, current, params_count) == -1)
			{
				std::string err = ERR_NEEDMOREPARAMS(c.getNickName(), "MODE");
				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
				return;
			}
		}
		else if(params[0][j] == 'l')
		{
			if (handle_Limit(Flag, params, current, params_count) == -1)
			{
				std::string err = ERR_NEEDMOREPARAMS(c.getNickName(), "MODE");
				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
				return;
			}
		}
		else
		{
			std::string err = ERR_UNKNOWNMODE(c.getNickName(), params[0][j]);
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		j++;
	}
	
}



//  params[0][j] == 't' || params[0][j] == 'k' ||  params[0][j] == 'l' || params[0][j] == 'o' 




void Server::Mode(std::vector<std::string> &params, Client &c)
{
	if (params.size() < 2)  // params
	{
		std::string err = ERR_NEEDMOREPARAMS(c.getNickName(), "MODE");
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (params[1][0] == '#') //incorrect channel Name
	{
		if (!has_theChannel(params[1]))  //incorrect channel
		{
			std::string err = ERR_NOSUCHCHANNEL(c.getNickName(), params[1]);
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		Channel &channel = get_channel(params[1]);
		if (!channel.Is_ClientInChannel(c)) // Client is not in channel
		{
			std::string err = ERR_NOTONCHANNEL(c.getNickName(), channel.getChannelName());
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		if (!channel.Is_OperatorInChannel(c)) // Client is an operator in channel
		{
			std::string err = ERR_CHANOPRIVSNEEDED(c.getNickName(), channel.getChannelName());
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		else
		{
			params.erase(params.begin()); // remove the command MODE
			params.erase(params.begin()); // remove the command MODE
			channel.handleModeCommand(params, c); // handle the mode command
			channel.PrintChannelInfo();
		}

	}
	else
	{
		std::string err = ERR_NOSUCHCHANNEL(c.getNickName(), "MODE");   /// CHANGE ERROR !
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}

}