#ifndef SERVER_HPP
#define SERVER_HPP

#include  "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <sstream>
#include <cctype>
#include <algorithm>
#define MAX_CONNECTIONS 10
#define GREEN_P "\033[1;32m"
#define GREEN_S "\033[0m"
#define RED_P "\033[1;31m"
#define RED_S "\033[0m"
#define INIT "-----------Server initialisation----------\n"

class Channel;
class Server
{
private:
	std::string							_name;
	std::string							_password;
	unsigned short						_port;
	Socket								_serverSocket;
	std::vector<Client>					_allClients;
	static std::vector<struct pollfd>	_pfds;
	char								_buffer[BUFFER_SIZE];
	std::vector<Channel>				_channels;
	
public:
	Server();
	Server(std::string name, std::string password, unsigned short port);
	~Server();
	void								initServer();
	void								setupServer();
	void								startListening();
	void								acceptConnection();
	void								receiveData(int fd);
	static void 						quitServer();
	Socket								&getServerSocket();
	std::vector<Client> 				getAllClients();
	Client								*getClientByFd(int fd);
	void								handlePassCommand(std::vector<std::string> &params, Client *c);
	void 								parseParams(std::vector<std::string> &params, Client *c);
	void 								parseMessage(std::string &parse, Client *c);
	int									nickNameAlreadyExists(std::string const &nick);
	void 								handleNickNameCommand(std::vector<std::string> &params, Client *c);
	void 								handleUserCommand(std::vector<std::string> &params, Client *c);
	void								Commands(std::vector<std::string> &params, Client *c);
	void								priv_msg(std::vector<std::string> &params, Client &c);
	std::vector<Channel> 				&getChannels();
	Channel								&get_channel( std::string &channel_name);
	Client								&get_client( std::string &nickName);
	bool 								has_theChannel( std::string channelName);
	bool 								has_theClient( std::string &nickName);
	void 								SendPrivMsg_User( std::string &target_name, const std::string &message, Client &c);
	void 								SendPrivMsg_Channel(std::string &target_name, const std::string &message, Client &c);
	void 								join(std::vector<std::string> &params, Client &c);
	void 								join_each_channel( std::string &channelName, Client &c, std::string password);
	Channel								&GetOrCreateChannel(const std::string &channelName);
	void 								message_to_Allclients( std::string &channelName, const std::string &message);
	void 								message_to_Channel( std::string &channelName, const std::string &message, Client &c);
	void 								SendChannelInfos(std::string &channelName, Client &c);
	bool 								valid_joining_channel(Channel &channel, Client &c, std::string password);
	void 								kick_from_channel(std::vector<std::string> &params, Client &c);
	void 								kick_by_one(std::string client_to_kick, Client &c, Channel &channel, std::vector<std::string> &params, std::string &ChannelName);
    void								Mode(std::vector<std::string> &params, Client &c);
    void								Invite_Command(std::vector<std::string> &params, Client &c);
	int									initialParsingMode(std::vector<std::string> &params, Client &c);
	void								displayMode(Client &c, Channel channel, Server &server);
	void								Topic_Handler(std::vector<std::string> &params, Client &c);
	std::string							getNickNameById(size_t id);
	size_t 								getIdByName(std::string &nickName) const;
	void								broadCastMessage(std::string &message, std::string old, Client &c);
	Client								&get_clientFound( std::string &nickName, size_t &found);
};

int							identifyCommand(std::string cmd);
std::string					extractMessage(std::string m);
void						signalHander(int sig);
std::string					get_corr_message(std::vector<std::string> &params, int index);
std::vector<std::string>	splitBy_delimeter(const std::string &str, char del);

#endif