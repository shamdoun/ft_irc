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
		std::string pass_as_stirng;
		int 	Limit;
		time_t TimeOfCreatiion;


	public:
		Channel(std::string name);

		std::vector<Client>& getClients() ;
		std::vector<std::string>& getOpperators();
		int getClientSize_inChannel() const;
		std::string getChannelName() const;
		bool Is_ClientInChannel(Client &client);
		bool Is_ClientInChannel_2(std::string NickName);
		
		void addAsOperator(Client &client);
		void addAsClient(Client &client);
		bool Is_OperatorInChannel(Client &client);  
		bool Is_OperatorInChannel_2(std::string &nickName) ;
		void RemoveOperator(std::string &nickName);
		void RemoveClient(std::string &nickName);
		

		// void addClient(Client *client);


		// MODE ===================================================================
		void	handleModeCommand(std::vector<std::string> &params, Client &c);
		bool	getterInvite(void);
		bool	getterTopic(void);
		bool	getterPasswd(void);
		int		getterLimit(void);
		std::string &getterPassAsString(void);
		void	SetterInvite(bool Value_To_Give);
		void	SetterTopic(bool Value_To_Give);
		void	SetterPasswd(bool Value_To_Give);
		void	SetterLimit(int Value_To_Give);
		void	SetterPassAsString(std::string Value_To_Give);
		void	handleInvite(int flag);
		void	handleTopicPermission(int flag);
		int		handle_Password(int flag, std::vector<std::string> &params, int	&current, int prms_count);
		int		handle_Limit(int flag, std::vector<std::string> &params, int	&current, int prms_count);
		void	PrintChannelInfo(void);
		void	message_to_channel2(std::string &message, Client &c);
		std::string getChannelMode(void);
		time_t	getCreationTime(); 

};

#endif

