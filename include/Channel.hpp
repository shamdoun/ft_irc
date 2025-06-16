#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"
#include <vector>

class Channel
{
	private:
		std::string _Channel_name;
		std::vector<Client*> _Clients; // Pointers to clients in the channel
		std::vector<std::string> _Operators; // Operators of the channel
		std::string _topic;
	public:
		Channel(std::string name);

		std::vector<Client*> getClients() const;
		int getClientSize() const;
		std::string getChannelName() const;
		void addAsOperator(Client *client);
		bool alreadyInChannel(Client *client) const;
		void addAsClient(Client *client);
		
		// void addClient(Client *client);
};

#endif