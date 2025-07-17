#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/numericReplies.hpp"
#include "../include/Channel.hpp"


void	Server::Topic_Handler(std::vector<std::string> &params, Client &c)
{
	// std::cout << params.size() <<  "PARAM SIZE \n";
	// if (params.size() <= 2 )
	// {
	// 	std::string err = ERR_NEEDMOREPARAMS(params[0]);
	// 	send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
	// 	return;
  	// }
	if (params.size() < 2 || (params.size() <= 2 && params[1] == ":"))
	{
		std::string err = ERR_NEEDMOREPARAMS(params[0]);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
  	}
	if (!has_theChannel(params[1]))
	{
		std::string err = ERR_NOSUCHCHANNEL(params[1]);
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;
	}
	Channel &channel = get_channel(params[1]);
	if (!channel.Is_ClientInChannel(c))
	{
		std::string err = ERR_NOTONCHANNEL(c.getNickName(), channel.getChannelName());
		send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
		return;	
	}
	if (params.size() == 2)
	{
		if (channel.getTopic() == "")
		{
			std::string err = RPL_NOTOPIC(c.getNickName(), channel.getChannelName());
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
		else
		{
			std::ostringstream oss;
			oss << channel.getTopicUpdateTime();
			std::string timeStr = oss.str();
			std::string reply = RPL_TOPIC(c.getNickName(), channel.getChannelName(), channel.getTopic());
			std::string nickname = c.getNickName();
			std::string channelName = channel.getChannelName();
			std::string timeReply = RPL_TOPICWHOTIME(nickname, channelName,	channel.WhoUpdatedGetter(), timeStr);
			send(c.getClientSocket().getSocketFd(), reply.c_str(), reply.size(), 0);
			send(c.getClientSocket().getSocketFd(), timeReply.c_str(), timeReply.size(), 0);
			return;
		}
	}
	else
	{
		if (channel.Is_OperatorInChannel(c) || !channel.getterTopic())
		{
			channel.SetterTopicAsString(get_corr_message(params, 2));
			channel.TopicUpdateTime();
			channel.WhoUpdatedSetter(c.getNickName());
			std::string Message = RPL_TOPIC(c.getNickName(), channel.getChannelName(), channel.getTopic());
			std::string Channel_Name = channel.getChannelName();
			message_to_Channel(Channel_Name, Message, c);
		}
		else
		{
			std::string err = ERR_CHANOPRIVSNEEDED(channel.getChannelName());
			send(c.getClientSocket().getSocketFd(), err.c_str(), err.size(), 0);
			return;
		}
	}
}
