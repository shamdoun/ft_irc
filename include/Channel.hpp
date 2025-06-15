#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"
#include <vector>

class Channel
{
	private:
		std::string _name;
		std::vector<Client*> _Clients; // Pointers to clients in the channel
		std::string _topic;
	public:
		Channel(std::string name);

		std::vector<Client*> getClients() const;
		int getClientSize() const;
		std::string getChannelName() const;
		bool is_member(Client *client) const;
		// void addClient(Client *client);
};

#endif