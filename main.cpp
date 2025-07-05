#include "include/Client.hpp"
#include "include/Server.hpp"

int isValidPort(char *p)
{
	int i = 0;
	double v;
	while (p[i])
	{
		if (!isdigit(p[i]))
			return (0);
		i++;
	}
	v = std::atof(p);
	if (v > 65535)
		return (0);
	return (1);
}

int isValidPassword(char *pass)
{
	int i = 0;
	bool	onlySpaces = true;
	while (pass[i])
	{
		if (!std::isspace(pass[i]))
			onlySpaces = false;
		i++;
	}
	return (onlySpaces ? 0 : 1);
}

int pasrseArgs(char *pass, char *port)
{
	if (!isValidPort(port))
	{
		std::cerr << "a valid range for the port is 0 - 65535\n";
		return (1);
	}
	if (!isValidPassword(pass))
	{
		std::cerr << "the password cannot be empty spaces!\n";
		return (1);
	}
	return (0);
}
int main(int argc, char **argv)
{
	unsigned short port;
	if (argc != 3)
	{
		std::cerr << "insuficiant number of args" << std::endl;
		return (1);
	}
	if (pasrseArgs(argv[2], argv[1]))
		exit(1);
	std::string password(argv[2]);
	port = std::atoi(argv[1]);
	signal(SIGINT, signalHander);
	signal(SIGTSTP, signalHander);
	Server s("test", password, port);
	try
	{
		s.initServer();
	}
	catch (std::exception &e)
	{
		std::cout << "error: " << e.what() << std::endl;
	}
	s.quitServer();
}
