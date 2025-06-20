#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"
#include <vector>

class Channel
{
	private:
		std::string _Channel_name;
		std::vector<Client> _Clients;
		std::vector<std::string> _Operators; // Operators of the channel
		std::string _topic;
		bool	Invite; //false
		// l = infinty 5 =  limit = 5


	public:
		Channel(std::string name);

		std::vector<Client>& getClients() ;
		int getClientSize_inChannel() const;
		std::string getChannelName() const;
		bool Is_ClientInChannel(Client &client);
		
		void addAsOperator(Client &client);
		void addAsClient(Client &client);
		bool Is_OperatorInChannel(Client &client);  

};

#endif