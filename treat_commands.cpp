#include "include/Client.hpp"
#include "include/Server.hpp"
#include "include/numericReplies.hpp"

void Server::Commands(std::vector<std::string> &params, Client *c)
{	
	if (c == NULL)
		return; // Handle case where client pointer is null
	Client &client = *c;
	if (params[0] == "PRIVMSG"|| params[0] == "privmsg")
		Server::priv_msg(params, client);
	if (params[0] == "JOIN" || params[0] == "join")
		Server::join(params, client);
	if (params[0] == "MODE" || params[0] == "mode")
		Server::Mode(params, client);
	
		
}