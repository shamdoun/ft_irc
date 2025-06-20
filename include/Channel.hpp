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
		bool	Topic;
		bool	Passwd;
		int 	Limit;


	public:
		Channel(std::string name);

		std::vector<Client>& getClients() ;
		int getClientSize_inChannel() const;
		std::string getChannelName() const;
		bool Is_ClientInChannel(Client &client);
		
		void addAsOperator(Client &client);
		void addAsClient(Client &client);
		bool Is_OperatorInChannel(Client &client);  
		// void addClient(Client *client);


		// MODE ===================================================================
		void	handleModeCommand(std::vector<std::string> &params, Client *c);
		bool	getterInvite(void);
		bool	getterTopic(void);
		bool	getterPasswd(void);
		int		getterLimit(void);
		void	SetterInvite(bool Value_To_Give);
		void	SetterTopic(bool Value_To_Give);
		void	SetterPasswd(bool Value_To_Give);
		void	SetterLimit(int Value_To_Give);
		void	PrintChannelInfo(void);

};

#endif