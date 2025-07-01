#pragma once
#include "Server.hpp"

int isValidNickname(std::string const &nick);
bool isValidUsername(const std::string& username);
void sendError(std::string err, Client *c);
