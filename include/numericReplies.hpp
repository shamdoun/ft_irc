#pragma once


#define SERVER_NAME "IRCServer"
#define PREFIX ":" SERVER_NAME " "

#define POSTFIX "\r\n"
#define RPL_WELCOME(nick, hostname)  std::string("\r\n:") + std::string(hostname) + std::string(" 001 ") + std::string(nick) + std::string(" :Welcome to the Internet Relay Chat Network ") + std::string(nick) + "!~" + std::string(nick) + "@" + std::string(hostname) + "\r\n"
#define RPL_YOURHOST(nick, hostname) std::string(":") + std::string(hostname) + std::string(" 002 ") + std::string(nick) + std::string(" :Your host is ") + std::string(hostname) + std::string(", running version 1.0\r\n")
#define RPL_CREATED(nick, hostname)  std::string(":") + std::string(hostname) + std::string(" 003 ") + std::string(nick) + std::string(" :This server was created Wed Jan 01 2025\r\n")
#define RPL_MYINFO(nick, hostname)   std::string(":") + std::string(hostname) + std::string(" 004 ") + std::string(nick) + " " + std::string(hostname) + std::string(", Version: 1.0, User mode: none, Channel modes: i, t, k, o, l\r\n")
#define ERR_NICKNAMEINUSE(target)    PREFIX "433 " + target + " :Nickname is already in use" POSTFIX
#define ERR_USERONCHANNEL(channel, target)    PREFIX "443 " + channel + " " + target + " :is already on channel" POSTFIX
#define ERR_NEEDMOREPARAMS(command)    PREFIX "461 " + command + " :Not enough parameters" POSTFIX  
#define ERR_UNKNOWNCOMMAND(command)    PREFIX "421 " + command + " :Unknown command" POSTFIX 
#define ERR_UNKNOWNBOTCOMMAND(command)    PREFIX "421 " + command + " :Unknown bot command" POSTFIX 
#define ERR_CHANOPRIVSNEEDED(channel)    PREFIX "482 " + channel + " :You're not channel operator" POSTFIX
#define ERR_ERRONEUSNICKNAME(nick)    PREFIX "432 " + nick + " :Erroneus nickname" POSTFIX
#define ERR_NOTEXTTOSEND(nick)PREFIX " 412 " + nick + " :No text to send" POSTFIX
#define ERR_NOSUCHCHANNEL(channel)    PREFIX "403 " + channel + " :No such channel" POSTFIX
#define ERR_INPUTTOOLONG(nick) PREFIX " 417 " + nick + " :Input line too long" POSTFIX
#define ERR_NOSUCHNICK(nick, target) PREFIX "401 " + nick + " " + target + " :No such nick/channel" POSTFIX
#define ERR_NOTONCHANNEL(target, channel)    PREFIX "442 " + target + " " + channel + " :You're not on that channel" POSTFIX
#define ERR_USERNOTINCHANNEL(user, channel)     PREFIX "441 " + user + " " + channel + " :They aren't on that channel" POSTFIX
#define ERR_NONICKNAMEGIVEN(target)               PREFIX "431 " + target + " :No nickname given" POSTFIX
#define ERR_PASSWDMISMATCH(target)    PREFIX "464 " + target + " :Password incorrect" POSTFIX
#define ERR_ALREADYREGISTERED(target)PREFIX "462 " + target + " :You may not re register" POSTFIX 
#define ERR_BADCHANNELKEY(channel)    PREFIX "475 " + channel + " :Cannot join channel (+k)" + POSTFIX
#define ERR_NOTREGISTERED(nickname)                 PREFIX "451 " + nickname + " :You have not registered!" POSTFIX
#define ERR_CHANNELISFULL(client, channel)PREFIX " 471 " + client + " " + channel + " :Cannot join channel (+l)" POSTFIX
#define ERR_INVITEONLYCHAN(client, channel)PREFIX " 473 " + client + " " + channel + " :Cannot join channel (+i)" POSTFIX
#define ERR_UNKNOWNMODE( nickname, chName, mode )	   ( ": 472 " + nickname + " " + chName + " " + mode + " :is not a recognised channel mode" + POSTFIX )
#define ERR_INVALIDMODEPARM(nick, channel, mode, param) (": 696 " + nick + " " + channel + " " + mode + " " + param + " :Invalid parameter for mode" + POSTFIX)
#define RPL_KICK(kikker, target, channel, reason)    ":" +  kikker +" KICK " + channel + " " + target + " :" + reason + POSTFIX
#define RPL_NAMREPLY(sender, channel, users)    PREFIX "353 " + sender + " = " + channel + " :" + users + POSTFIX
#define RPL_ENDOFNAMES(sender, channel)        PREFIX "366 " + sender + " " + channel + " :End of /NAMES list." POSTFIX
#define RPL_TOPIC(sender, channel, topic)PREFIX " 332 " + sender + " " + channel + " :" + topic + POSTFIX
#define RPL_PRIVMSG(sender, username, ip, target, msg)":" + sender + "!" + username + "@" + ip + " PRIVMSG " + target + " :" + msg + POSTFIX 
#define RPL_NICK(sender, nick)":" + sender + " NICK " + nick + POSTFIX
#define RPL_NOTOPIC(sender, channel) PREFIX "331 " + sender + " " + channel + " :No topic is set" + POSTFIX
#define RPL_INVITING(nickname, targnick, targchan)  ": 341 " + nickname + " " + targnick + " " + targchan + POSTFIX
#define RPL_INVITE(sender, target, channel)":" + sender + " INVITE " + target + " " + channel + POSTFIX
#define RPL_INVITING(nickname, targnick, targchan)  ": 341 " + nickname + " " + targnick + " " + targchan + POSTFIX
#define RPL_JOINMSG(hostname, ipaddress, channelname)(":" + hostname + ipaddress + " JOIN " + channelname + POSTFIX)
#define RPL_JOIN(sender, channel)":" + sender + " JOIN :" + channel + POSTFIX
#define RPL_CHANGEMODE(hostname, channelname, mode)(":" + hostname + " MODE " + channelname + " +" + mode + POSTFIX)
#define RPL_CHANGEMODEMINUS(hostname, channelname, mode)(":" + hostname + " MODE " + channelname + " -" + mode + POSTFIX)
#define RPL_UMODEIS(hostname, channelname, mode, user)":" + hostname + " MODE " + channelname + " +" + mode + " " + user + POSTFIX
#define RPL_UMODEISMINUS(hostname, channelname, mode, user)":" + hostname + " MODE " + channelname + " -" + mode + " " + user + POSTFIX
#define RPL_PART(sender, channel, reason)    ":" + sender + " PART " + channel + " :" + reason + POSTFIX
#define RPL_QUIT(sender, reason)                        ":" + sender + " QUIT :Quit: " + reason + POSTFIX
#define RPL_HELPSTART(nick, subject, txt)               PREFIX "704 " + nick + " " + subject + " :" + txt + POSTFIX
#define RPL_HELPTXT(nick, subject, txt)               PREFIX "705 " + nick + " " + subject + " :" + txt + POSTFIX
#define RPL_ENDOFHELP(nick, subject)               PREFIX "706 " + nick + " " + subject + " :" + "End of /HELP." + POSTFIX
#define ERR_ERRONEUSUSERNAME(userName) ": 432 " + userName + " :Erroneous username" + POSTFIX
#define ERR_NORECIPIENT(nickName) PREFIX ": 411 " + nickName + " :No recipient given (PRIVMSG)" + POSTFIX
#define RPL_CHANNELMODEIS(nickName, channelName, modes) ": 324 " + nickName + " " + channelName + " " + modes + POSTFIX
#define RPL_CREATIONTIME(nickName, channelName, time) ": 329 " + nickName + " " + channelName + " " + time + POSTFIX
#define RPL_TOPICWHOTIME(nickName, channelName, who, time) ": 333 " + nickName + " " + channelName + " " + who + " " + time + POSTFIX