#include "include/Client.hpp"
#include "include/Server.hpp"
#include "include/numericReplies.hpp"

void Server::Commands(std::vector<std::string> &params, Client *c)
{
	if (params[0] == "PRIVMSG")
		Server::priv_msg(params, c);
	else if (params[0] == "JOIN")
		Server::join(params, c);
		
}