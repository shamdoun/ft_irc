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
#define MAX_CONNECTIONS 5



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

	std::vector<Channel>					_channels; // List of channels on the server

public:
	Server();
	Server(std::string name, std::string password, unsigned short port);
	~Server();
	void								initServer();
	void								setupServer();
	void								startListening();
	void								acceptConnection();
	void								disconnectUser(int fd);
	void								receiveData(int fd);
	void								sendData();
	static void 						quitServer();
	Socket								&getServerSocket();
	std::string 						getName();
	std::string 						getPassword();
	unsigned short						getPort();
	std::vector<Client> 				getAllClients();
	static std::vector<struct pollfd>	getPfds();
	char								*getBuffer();
	Client								*getClientByFd(int fd);
	void								handlePassCommand(std::vector<std::string> &params, Client *c);
	void 								parseParams(std::vector<std::string> &params, Client *c);
	void 								parseMessage(char *buf, Client *c);
	int									nickNameAlreadyExists(std::string const &nick);
	void 								handleNickNameCommand(std::vector<std::string> &params, Client *c);
	void 								handleUserCommand(std::vector<std::string> &params, Client *c);
	
	// member functions (Anass)
	void Commands(std::vector<std::string> &params, Client *c);
	// void parseMessage(char *buf, Client *c);
	// void parseParams(std::vector<std::string> &params, Client *c);
	void SendPrivMsg_User(const std::string &target_name, const std::string &message, Client *c);
	Client *getClientByNickName(std::string nickName);
	void priv_msg(std::vector<std::string> &params, Client *c);
	void SendPrivMsg_Channel(const std::string &target_name, const std::string &message, Client *c);
	void join(std::vector<std::string> &params, Client *c);
	std::vector<Channel> getChannels() const { return _channels; }
	Channel *GetOrCreateChannel(const std::string &channelName);
	void join_each_channel(const std::string &channelName, Client *c, const std::string &password);
	bool valid_joining_channel(Channel *channel, Client *c, const std::string &password);
	Channel *getChannelByName(const std::string &channelName);


};

int	identifyCommand(std::string cmd);
std::string extractMessage(std::string m);
void signalHander(int sig);
#endif