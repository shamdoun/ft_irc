#include "../include/Socket.hpp"
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "../include/numericReplies.hpp"
#include <exception>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <algorithm>
#include <vector>
#include <signal.h>
#include <string>

class Bot
{
    private:
		static Socket	_clientSocket;
        std::string		_ip;
        unsigned short	_port;
        std::string		_password;
		std::string 	_nick;
		std::string 	_user;
		char			_buffer[1024];
    public:
        Bot();
        ~Bot();
        Bot(std::string _ip, unsigned short port, std::string _password, std::string _nick);
		Bot(const Bot &other);
		Bot 		&operator=(const Bot &other);
		void		run();
		static void	quit();
		void 		handleBotRequest(std::string buf);
		void		createBot();
		void		authenticate();
		void		hello(std::string &receiver);
		void		echo(std::string receiver, std::vector<std::string> args);
		void		help(std::string &receiver);
		void		quote(std::string &receiver);
		void		sendTime(std::string &receiver);
		void		sendMessage(std::string msg, std::string receiver);
};