#include "Bot_bonus.hpp"


void sigHandler(int sig)
{
    (void)sig;
    Bot::quit();
    exit(0);
}

static void parseArgs(std::string &ip, std::string port)
{
    int p = std::atoi(port.c_str());
    if (ip == "localhost" || ip == "LOCALHOST")
    {
        ip = "0.0.0.0";
    }
    if ((port.find_first_not_of("0123456789") != std::string::npos) || p < 1024 || p > 65535)
    {
        std::cerr << "incorrect range for port, it should be 1024 <= p <= 65535\n";
        exit(1);
    }
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "Insuficent number of args!\n";
        return (1);
    }
    std::string ip(argv[1]);
    parseArgs(ip, std::string(argv[2]));
    signal(SIGINT, sigHandler);
    signal(SIGTSTP, sigHandler);
    signal(SIGHUP, sigHandler);
	signal(SIGPIPE, SIG_IGN);
    unsigned short  port = std::atoi(argv[2]);
    std::string password(argv[3]);
    try
    {
        Bot bot(ip, port, password);
        bot.createBot();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    Bot::quit();
}