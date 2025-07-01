#pragma once
#include <string>
class Client;

std::string extractMessage(std::string m);
void handleUnkownCommand(std::string cmd, Client *c);
