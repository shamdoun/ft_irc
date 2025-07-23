#include "Bot_bonus.hpp"

Socket Bot::_clientSocket;

Bot::Bot()
{
    _ip = "localhost";
    _port = 8000;
    _password = "default";
    _nick = "default";
    _user = "default";
}

Bot::~Bot()
{
    
}

Bot::Bot(std::string _ip, unsigned short port, std::string _password, std::string _nick):_ip(_ip),_port(port),_password(_password), _nick(_nick), _user("bot bot * bot")
{
}

Bot::Bot(const Bot &other)
{
    *this = other;
}

Bot &Bot::operator=(const Bot &other)
{
    if (this != &other)
    {
        _password = other._password;
        _clientSocket = other._clientSocket;
        _port = other._port;
        _ip = other._ip;
        _nick = other._nick;
        _user = other._user;
    }
    return (*this);
}

void Bot::createBot()
{
    int     fd;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    Socket serverSocket(1);
    
    if (setsockopt(_clientSocket.getSocketFd(), SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0)
        throw std::runtime_error("setsocketopt failed!");
    serverSocket.setSocketAdress(AF_INET, _port, inet_addr(_ip.c_str()));
    socklen_t len = sizeof(serverSocket.getSocketAddress());
    _clientSocket.setIpAddress(inet_ntoa(_clientSocket.getSocketAddress().sin_addr));
    fd = connect(_clientSocket.getSocketFd(), (sockaddr *)&serverSocket.getSocketAddress(), len);
    if (fd < 0)
        throw std::runtime_error("failed to connect the bot to the server!");
    authenticate();
    run();
}

void Bot::quit()
{
    std::cout << "\nquiting the bot....\n";
    _clientSocket.closeSocket();
}

void Bot::authenticate()
{
    int ret;
    std::string nickname = "nick " + _nick + POSTFIX;
    std::string pass = "pass " + _password + POSTFIX;
    std::string username = "user " + _user + POSTFIX;

    ret = send(_clientSocket.getSocketFd(), pass.c_str(), pass.size(), 0);
    if (ret < 0)
    {
        std::cerr << "failed to send nickname to server!\n";
    }
    usleep(200);
    ret = send(_clientSocket.getSocketFd(), nickname.c_str(), nickname.size(), 0);
    if (ret < 0)
        std::cerr << "failed to send nickname to server!\n";
    usleep(200);
    ret = send(_clientSocket.getSocketFd(), username.c_str(), username.size(), 0);
    if (ret < 0)
        std::cerr << "failed to send nickname to server!\n";
}

std::string extractMessage(std::string m)
{
    size_t end;
    end = m.find("\r\n");
    if (end != std::string::npos)
            return (m.substr(0, end));
	return m;
}

void Bot::run()
{
    int ret;
    bool registered = false;

    std::cout << "starting bot....\n";
    while (true)
    {
        memset(_buffer, 0, sizeof(_buffer));
        ret = recv(_clientSocket.getSocketFd(), _buffer, sizeof(_buffer), 0);
        if (ret < 0)
            std::cerr << "failed to receive data from server!\n";
        if (!ret)
        {
            Bot::quit();
            exit(0);
        }
        if (std::strstr(_buffer, "Welcome") && !registered)
            registered = true;
        else if(registered)
        {
            if (std::strstr(_buffer, "PRIVMSG"))
            {
                handleBotRequest(extractMessage(std::string(_buffer)));
            }
        }
        else
        {
            std::cout << "Error: " << _buffer;
            break ;
        }
        usleep(300);
    }
}

void Bot::sendMessage(std::string msg, std::string receiver)
{
    std::string fullMsg = "privmsg " + receiver + " :" + msg + POSTFIX;
    send(_clientSocket.getSocketFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

void Bot::sendTime(std::string &receiver)
{
    char buf[50];
    std::time_t now = std::time(0);
    std::tm *localtime = std::localtime(&now);
    memset(&buf, 0, sizeof(buf));
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime);
    sendMessage(buf, receiver);
}

void Bot::handleBotRequest(std::string buf)
{
    std::vector<std::string> params;
    std::string receiver;
    std::stringstream s(buf);
    std::string tmp;

    while (getline(s, tmp, ' '))
    {
        params.push_back(tmp);
    }
    size_t pos; 
    pos = params[0].find("!");
    receiver = params[0].substr(1, pos - 1);
    if (params[3] != ":echo" && params.size() > 4)
    {
        sendMessage("incorrect input, check `help` for available commands!!", receiver) ;
    }
    else if (params[3] == ":time")
    {
        sendTime(receiver);
    }
    else if (params[3] == ":echo")
    {
        echo(receiver, params);
    }
    else if (params[3] == ":quote")
    {
        quote(receiver);
    }
    else if (params[3] == ":hello")
    {
        hello(receiver);
    }
    else if (params[3] == ":help")
    {
        help(receiver);
    }
    else
    {
        sendMessage("incorrect input, check `help` for available commands!!", receiver) ;
    }
}

void Bot::echo(std::string receiver, std::vector<std::string> args)
{
    std::string msg;
    if (args.size() < 5)
    {
        sendMessage(std::string("no argument to echo!"), receiver);
    }
    else
    {
        for (size_t i = 4; i < args.size(); i++)
        {
            msg += args[i];
            if (i != (args.size() - 1))
                msg += " ";
        }
        sendMessage(msg, receiver);
    }
}

void Bot::quote(std::string &receiver)
{
    std::vector<std::string>    quotes;
    int                         random;

    quotes.push_back("“The only way to do great work is to love what you do.” — Steve Jobs");
    quotes.push_back("“In the middle of difficulty lies opportunity.” — Albert Einstein");
    quotes.push_back("“The purpose of life is not to be happy. It is to be useful, to be honorable, to be compassionate, to have it make some difference that you have lived and lived well.” — Ralph Waldo Emerson");
    quotes.push_back("“Success is not final, failure is not fatal: It is the courage to continue that counts.” — Winston Churchill");
    quotes.push_back("“It does not matter how slowly you go as long as you do not stop.” — Confucius");
    random = rand() % quotes.size();
    srand(time(0));
    std::string rQuote = quotes[random];
    sendMessage(rQuote, receiver);
}

void Bot::hello(std::string &receiver)
{
    std::string welcomeMsg("Hello! I'm your friendly bot here to help you get started. enter help to see possible commands!");
    sendMessage(welcomeMsg, receiver);
}

void Bot::help(std::string &receiver)
{
    std::string time("time: ⏰ Get the current time in [your time zone].\n");
    std::string echo("echo [message]: 💬 I'll repeat whatever you say! Just type a message after the command.\n");
    std::string quote("quote: Get a random inspiring or funny quote to brighten your day!\n");
    std::string help("help: Display this help message to learn about available commands and how to use them.\n");
    std::string hello("hello: Display welcome message!");

    sendMessage(time, receiver);
    usleep(100);
    sendMessage(echo, receiver);
    usleep(100);
    sendMessage(quote, receiver);
    usleep(100);
    sendMessage(help, receiver);
    usleep(100);
    sendMessage(hello, receiver);
}