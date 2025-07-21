#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"
#include <vector>
class Server;
class Channel
{
	private:
		std::string					_Channel_name;
		std::vector<Client>			_Clients;
		std::vector<Client>			_Operators;
		std::vector<Client>			_Invited;
		std::string					_topic;
		bool						Invite;
		bool						Topic;
		bool						Passwd;
		std::string					pass_as_stirng;
		int							Limit;
		time_t						TimeOfCreatiion;
		time_t						Topic_Update_Time;
		std::string					Client_who_updated;
		bool						stop_kick;

	public:
		Channel(std::string name);
		std::vector<Client>			&getClients() ;
		std::vector<Client>	&getOpperators();
		int							getClientSize_inChannel() const;
		std::string					getChannelName() const;
		bool 						Is_ClientInChannel(Client &client);
		bool						Is_ClientInChannel_2(size_t id);
		void						addAsOperator(Client &client);
		void						addAsClient(Client &client);
		bool						Is_OperatorInChannel(Client &client);  
		bool						Is_OperatorInChannel_2(size_t id) ;
		void						RemoveOperator(size_t id, Client &c);
		void						RemoveClient(size_t id);
		void						handleModeCommand(std::vector<std::string> &params, Client &c, Server &server);
		bool						getterInvite(void);
		bool						getterKickFlag(void);
		bool						getterTopic(void);
		bool						getterPasswd(void);
		int							getterLimit(void);
		std::string					&getterPassAsString(void);
		void						SetterKickFlag(bool Value_To_Give);
		void						SetterInvite(bool Value_To_Give);
		void						SetterTopic(bool Value_To_Give);
		void						SetterPasswd(bool Value_To_Give);
		void						SetterLimit(int Value_To_Give);
		void						SetterPassAsString(std::string Value_To_Give);
		std::string 				getTopic(void) const;
		void						SetterTopicAsString(std::string Value_To_Give);
		void						handleInvite(int flag);
		void						handleTopicPermission(int flag);
		int							handle_Password(int flag, std::vector<std::string> &params, int	&current, int prms_count);
		int							handle_Limit(int flag, std::vector<std::string> &params, int	&current, int prms_count);
		void						message_to_channel2(std::string &message, Client &c);
		std::string					getChannelMode(void);
		time_t						getCreationTime();
		time_t						getTopicUpdateTime();
		void						updateCreationTime();
		void						TopicUpdateTime();
		int							addAsInvited(Client &client);
		int							Is_Invited(Client &client);
		std::string 				WhoUpdatedGetter(void);
		void						WhoUpdatedSetter(std::string WhoChandedstr); //^ i should change it to saving the id or the client class not the nickname
		void						RemoveFromInvite(Client &client);
		bool						isAllDigits(const std::string& str);
	};

#endif

