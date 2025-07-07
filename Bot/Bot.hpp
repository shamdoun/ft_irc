#pragma once
#include "../include/Server.hpp"
#include <ctime>
#include <cstdlib>

void sendCurrentTime(Server *s, Client *c);
void echoMessage(Server *s, Client *c, std::string message);
void onlineUsers(Server *s, Client *bot, Client *c);
void quote(Server *s, Client *c);
void printBotMessage(Server *s, Client *bot, Client *c);
void printHelpMessage(Server *s, Client *bot, Client *c);
void checkBotCommand(std::string msg);