#include "Bot.hpp"
#include "../include/Authentification.hpp"
#include "../include/numericReplies.hpp"
#include <cstdio>

void Server::createBot()
{
	int     fd;
	Socket  c;
	struct pollfd pfd;
    Socket  serverSocket;
    std::string nick("~bot~"), user("bot"), host("localhost"), realName("bot");
     
    serverSocket = getServerSocket();
	socklen_t len = sizeof(serverSocket.getSocketAddress());
	fd = connect(c.getSocketFd(), (sockaddr *)&serverSocket.getSocketAddress(), len);
	if (fd < 0)
		throw std::runtime_error("failed to connect the bot to the server!");
	c.setIpAddress(inet_ntoa(c.getSocketAddress().sin_addr));
	Client bot(c);
    bot.setIsBot(true);
    bot.setNickName(nick);
    bot.setUserName(user);
    bot.setHostName(host);
    bot.setRealName(realName);
    bot.setIsAuthenticated(true);
    bot.setIsRegistered(true);
	_allClients.push_back(bot);
    pfd.fd = c.getSocketFd();
	pfd.events = POLL_IN;
	pfd.revents = 0;
	_pfds.push_back(pfd);
}

void Server::handleBotRequest(std::vector<std::string> &params, Client *c)
{
    if (!c->getIsAuthenticated())
    {
        sendError(ERR_NOTREGISTERED(c->getNickName()), c);
        return ;
    }
    Client *bot = getBot();
    if (params.size() < 2)
    {
        printBotMessage(this, bot, c);
        return ;
    }
    // if (params.size() > 2 && params[1] != "echo")
    // {
        // sendError(ERR_UNKNOWNCOMMAND(params[2] + " " + params[3]))
        // return ;
    // }
    if (params[1] == "help")
    {
        printHelpMessage(this, bot, c);
        return ;
    }
    if (params[1] == "online")
    {
        onlineUsers(this, bot, c);
        return ;
    }
    if (params[1] == "quote")
    {
        quote(this, bot, c);
        return ;
    }
    sendError(ERR_UNKNOWNBOTCOMMAND(params[1]), c);
    // if (params[1] == "echo")
    // {
    //     if (params.size() < 3)
    //         sendError(ERR_NEEDMOREPARAMS(params[1]), c);
    //     else
    //         echoMessage(this, c, params[3]);
    //     return ;
    // }
}

static bool ClientIsBot(Client c)
{
    return (c.getIsBot());
}

Client *Server::getBot()
{
    std::vector<Client>::iterator it = find_if(_allClients.begin(), _allClients.end(), ClientIsBot);
    return (it != _allClients.end() ? &(*it) : NULL);
}

// void sendCurrentTime(Server *s, Client *c)
// {

// }

// void echoMessage(Server *s, Client *c, std::string message)
// {

// }

void onlineUsers(Server *s, Client *bot, Client *c)
{
    int onlineUsers = s->getAllClients().size() - 2;
    char stringValue[50];
    std::string nick = c->getNickName();
    std::string output("Current online members: ");
    
    sprintf(stringValue, "%d", onlineUsers);
    output += std::string(stringValue);
    s->SendPrivMsg_User(nick, output, (*bot));
}

void quote(Server *s, Client *bot, Client *c)
{
    std::vector<std::string> quotes;

    quotes.push_back("“The only way to do great work is to love what you do.” — Steve Jobs");
    quotes.push_back("“In the middle of difficulty lies opportunity.” — Albert Einstein");
    quotes.push_back("“The purpose of life is not to be happy. It is to be useful, to be honorable, to be compassionate, to have it make some difference that you have lived and lived well.” — Ralph Waldo Emerson");
    quotes.push_back("“Success is not final, failure is not fatal: It is the courage to continue that counts.” — Winston Churchill");
    quotes.push_back("“It does not matter how slowly you go as long as you do not stop.” — Confucius");
    int random = rand() % quotes.size();
    srand(time(0));
    std::string rQuote = quotes[random];
    std::string nick = c->getNickName();
    s->SendPrivMsg_User(nick, rQuote, (*bot));
}

void printBotMessage(Server *s, Client *bot, Client *c)
{
    std::string nick = c->getNickName();
    std::string welcomeMsg("Hello! I'm your friendly bot here to help you get started. enter #help to see possible commands!");
    
    s->SendPrivMsg_User(nick, welcomeMsg, (*bot));
}

void printHelpMessage(Server *s, Client *bot, Client *c)
{
    std::string nick = c->getNickName();
    std::string time("\ntime: ⏰ Get the current time in [your time zone].\n");
    std::string echo("echo [message]: 💬 I'll repeat whatever you say! Just type a message after the command.\n");
    std::string online("online: See a list of users currently online in the server.\n");
    std::string quote("quote: Get a random inspiring or funny quote to brighten your day!\n");
    std::string help("help: Display this help message to learn about available commands and how to use them.\n");   

    s->SendPrivMsg_User(nick, time + echo + online + quote + help, (*bot));
}

// void checkBotCommand(std::string msg)
// {
//     std::string                 cmd;
//     std::vector<std::string>    params;
    
//     if (msg[0] != '#')
//     {
//         std::cerr << "error\n";
//     }
// }