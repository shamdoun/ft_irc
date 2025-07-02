#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"


// void	Channel::handleModeCommand(std::vector<std::string> &params, Client &c)
// {
// 	int	params_count = params.size() -1;
// 	int	Flag = 1;
// 	int	j = 0;
// 	int current = 1;
// 	std::string cmd = "MODE";

// 	std::cout << "Params_count ===> " << params_count << std::endl;
// 	while (params[0][j])
// 	{
// 		if (params[0][j] == '+')
// 			Flag = 1;
// 		else if(params[0][j] == '-')
// 			Flag = -1;
// 		else if(params[0][j] == 'i')
// 			this->handleInvite(Flag);
// 		else if(params[0][j] == 't')
// 			this->handleTopicPermission(Flag);
// 		else if(params[0][j] == 'k')
// 		{
// 			if (handle_Password(Flag, params, current, params_count) == -1)
// 			{
// 				std::string err = ERR_NEEDMOREPARAMS(cmd);
// 				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
// 				return;
// 			}
// 		}
// 		else if(params[0][j] == 'l')
// 		{
// 			if (handle_Limit(Flag, params, current, params_count) == -1)
// 			{
// 				std::string err = ERR_NEEDMOREPARAMS(cmd);
// 				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
// 				return;
// 			}
// 		}
// 		else if(params[0][j] == 'o')
// 		{
// 			if (current >= (int)params.size()) {
// 				std::string err = ERR_NEEDMOREPARAMS(cmd);
// 				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
// 				return;
// 			}
// 			std::string targetNick = params[current++];
// 			if (!this->Is_ClientInChannel_2(targetNick)) {
// 				std::string err = ERR_USERNOTINCHANNEL(targetNick, this->getChannelName());
// 				send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
// 				return;
// 			}
// 			if (Flag == 1) {
// 				if (!this->Is_OperatorInChannel_2(targetNick)) {
// 					this->_Operators.push_back(targetNick);
// 				}
// 			} else if (Flag == -1) {
// 				for (std::vector<std::string>::iterator it = this->_Operators.begin(); it != this->_Operators.end(); ++it) {
// 					if (*it == targetNick) {
// 						this->_Operators.erase(it);
// 						break;
// 					}
// 				}
// 			}
// 		}
// 		else
// 		{
// 			std::string err = ERR_UNKNOWNMODE(c.getNickName(), this->getChannelName(), params[0][j]);
// 			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
// 			return;
// 		}
// 		j++;
// 	}
	
// }


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
            Flag = 1; // Set mode
        else if(params[0][j] == '-')
            Flag = -1; // Unset mode
        else if(params[0][j] == 'i')
		{
			std::string Message;
            if (Flag == 1)
            {
                Message = c.getNickName() + " MODE " +  this->getChannelName() + " +" + std::string(1, params[0][j]) + "\r\n";
				std::cout << "Setting invite-only mode for channel: " << this->getChannelName() << std::endl;
			}
            else if (Flag == -1)
            {
                Message = c.getNickName() + " MODE "  + this->getChannelName() + " -" + std::string(1, params[0][j]) +  "\r\n";
				std::cout << "Removing invite-only mode from channel: " << this->getChannelName() << std::endl;
			}
			
			// else
			// {
			// 	std::string err = ERR_UNKNOWNMODE(c.getNickName(), this->getChannelName(), params[0][j]);
			// 	send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			// 	return;
			// }
			send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
            this->handleInvite(Flag); // Set/remove invite-only channel mode (+i/-i)
		}
        else if(params[0][j] == 't')
		{
            this->handleTopicPermission(Flag); // Set/remove topic protection (+t/-t)
			if (Flag == 1)
			{
				std::string Message = c.getNickName() + " MODE " + std::string(1, params[0][j]) + " " + this->getChannelName() + " +" + "\r\n";
				send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
				std::cout << "Setting topic protection for channel: " << this->getChannelName() << std::endl;
			}
			else if (Flag == -1)
			{
				std::string Message = c.getNickName() + " MODE " + std::string(1, params[0][j]) + " " + this->getChannelName() + " -" + "\r\n";
				send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
				std::cout << "Removing topic protection from channel: " << this->getChannelName() << std::endl;
			}
			
		}
        else if(params[0][j] == 'k')
        {
            if (handle_Password(Flag, params, current, params_count) == -1)
            {
                std::string err = ERR_NEEDMOREPARAMS(cmd); // ERR_NEEDMOREPARAMS (RFC 2812 461)
                send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
                return;
            }
			if (Flag == 1)
			{
				std::string Message = c.getNickName() + " MODE " + std::string(1, params[0][j]) + " " + this->getChannelName() + " +" + this->getterPassAsString() + "\r\n";
				send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
				std::cout << "Setting password for channel: " << this->getChannelName() << std::endl;
			}
			else if (Flag == -1)
			{
				std::string Message = c.getNickName() + " MODE " + std::string(1, params[0][j]) + " " + this->getChannelName() + " -" + "\r\n";
				send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
				std::cout << "Removing password from channel: " << this->getChannelName() << std::endl;
			}
			
        }
        else if(params[0][j] == 'l')
        {
            // Set/remove user limit (+l/-l), needs parameter for +l
            if (handle_Limit(Flag, params, current, params_count) == -1)
            {
                std::string err = ERR_NEEDMOREPARAMS(cmd); // ERR_NEEDMOREPARAMS (RFC 2812 461)
                send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
                return;
            }
			if (Flag == 1)
			{
				std::string Message = c.getNickName() + " MODE " + std::string(1, params[0][j]) + " " + this->getChannelName() + " + " "\r\n";
				send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
				std::cout << "Setting user limit for channel: " << this->getChannelName() << std::endl;
			}
			else if (Flag == -1)
			{
				std::string Message = c.getNickName() + " MODE " + std::string(1, params[0][j]) + " " + this->getChannelName() + " -" + "\r\n";
				send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
				std::cout << "Removing user limit from channel: " << this->getChannelName() << std::endl;
			}
		}
        else if(params[0][j] == 'o')
        {
            // Set/remove channel operator (+o/-o), needs parameter (nickname)
            if (current >= (int)params.size())
			{
                std::string err = ERR_NEEDMOREPARAMS(cmd); // ERR_NEEDMOREPARAMS (RFC 2812 461)
                send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
                return;
            }
            std::string targetNick = params[current++];
            if (!this->Is_ClientInChannel_2(targetNick))
			{
                std::string err = ERR_USERNOTINCHANNEL(targetNick, this->getChannelName()); // ERR_USERNOTINCHANNEL (RFC 2812 441)
                send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
                return;
            }
            if (Flag == 1) {
                if (!this->Is_OperatorInChannel_2(targetNick))
				{
                    this->_Operators.push_back(targetNick);
					std::cout << "Added operator: " << targetNick << " to channel: " << this->getChannelName() << std::endl;
					std::string Message = c.getNickName() + " MODE " + std::string(1, params[0][j]) + " " + this->getChannelName() + " +" + targetNick + "\r\n";
					send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
				}
            } else if (Flag == -1)
			{
                for (std::vector<std::string>::iterator it = this->_Operators.begin(); it != this->_Operators.end(); ++it) {
                    if (*it == targetNick)
					{
                        this->_Operators.erase(it);
						std::cout << "Removed operator: " << targetNick << " from channel: " << this->getChannelName() << std::endl;
						std::string Message = c.getNickName() + " MODE " + std::string(1, params[0][j]) + " " + this->getChannelName() + " -" + targetNick + "\r\n";
						send(c.getClientSocket().getSocketFd(), Message.c_str(), Message.size(), 0);
						break;
					}
                }
            }
        }
        else
        {
            std::string err = ERR_UNKNOWNMODE(c.getNickName(), this->getChannelName(), params[0][j]); // ERR_UNKNOWNMODE (RFC 2812 472)
            send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
            return;
        }
        j++;
    }
    
}



void Server::Mode(std::vector<std::string> &params, Client &c)
{
	if (params.size() < 2)  // params
	{
		std::string err = ERR_NEEDMOREPARAMS(params[0]);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (params[1][0] == '#') //incorrect channel Name
	{
		if (!has_theChannel(params[1]))  //incorrect channel
		{
			std::string err = ERR_NOSUCHCHANNEL(params[1]);
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
			std::string err = ERR_CHANOPRIVSNEEDED(channel.getChannelName());
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		else
		{
			params.erase(params.begin()); // remove the command MODE
			params.erase(params.begin()); // remove the command MODE
			channel.handleModeCommand(params, c); // handle the mode command
			// channel.PrintChannelInfo();
		}

	}
	else
	{
		std::string err = ERR_NOSUCHCHANNEL(params[1]);   /// CHANGE ERROR !
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}

}