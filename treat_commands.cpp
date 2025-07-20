#include "include/Client.hpp"
#include "include/Server.hpp"
#include "include/numericReplies.hpp"

void Server::Commands(std::vector<std::string> &params, Client *c)
{	
	std::string err;
	if (!c->getIsAuthenticated())  
	{   
		err = ERR_NOTREGISTERED(c->getNickName());
		if (send(c->getClientSocket().getSocketFd(), err.c_str(), err.size(), 0) < 1)   
		{    
			std::cerr << "failed to send " << err << std::endl;
		}   return ;
	} 
	if (c == NULL)
		return;
	Client &client = *c;
	if (params[0] == "PRIVMSG"|| params[0] == "privmsg")
		Server::priv_msg(params, client);
	if (params[0] == "JOIN" || params[0] == "join")
		Server::join(params, client);
	if (params[0] == "MODE" || params[0] == "mode")
		Server::Mode(params, client);
	if (params[0] == "KICK" || params[0] == "kick")
		Server::kick_from_channel(params, client);
	if (params[0] == "INVITE" || params[0] == "invite")
	    Server::Invite_Command(params, client);
	if (params[0] == "TOPIC" || params[0] == "topic")
	    Server::Topic_Handler(params, client);
		
}