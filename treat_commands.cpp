// #include "include/Client.hpp"
// #include "include/Server.hpp"

// void Server::SendPrivMsg_User(const std::string &target_name, const std::string &message, Client *c)
// {
// 	Client *target_client = Server::getClientByNickName (target_name);
// 	if (target_client)
// 	{
// 		std::string response = ":" + c->getNickName() + " PRIVMSG " + target_name + " :" + message + "\r\n";
// 		send(target_client->getClientSocket().getSocketFd(), response.c_str(), response.size(), 0);
// 	}
// 	else
// 	{
// 		std::cerr << "User " << target_name << " not found." << std::endl;
// 	}
// }

// // void SendPrivMsg_Channel(const std::string &target_name, const std::string &message, Client *c)
// // {

// // }
// void Server::commands(std::vector<std::string> &params, Client *c)
// {
// 	// std::cout << "Received command: " << params[0] << std::endl;
// 	// (void)	c; // Assuming 'c' is not used in this function, remove if necessary
// 	if (params[0] == "PRIVMSG")
// 	{
// 		if (params.size() < 3)
// 		{
// 			std::cerr << "PRIVMSG command requires at least 2 parameters" << std::endl;
// 			return;
// 		}
// 		std::string target_name = params[1];
// 		std::string message ;
// 		if (params[2][0] == ':')
// 		{
// 			for (size_t i = 2; i < params.size(); ++i)
// 			{
// 				message += params[i];
// 				if (i < params.size() - 1)
// 					message += " ";
// 			}
// 			message.erase(0, 1); // Remove the leading ':'
// 		}
// 		else
// 			message = params[2];
// 		// if (target_name[0] == '#')
// 		// 	SendPrivMsg_Channel(target_name, message, c);
// 		// else
// 			Server::SendPrivMsg_User(target_name, message, c);
// 	}
		
// }