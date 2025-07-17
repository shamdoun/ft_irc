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
#define ERR_NOTEXTTOSEND(nick)PREFIX "412 " + nick + " :No text to send" POSTFIX
// #define ERR_NORECIPIENT(nick, command)              PREFIX "411 " + nick + " :No recipient given (" + command + ")" POSTFIX
#define ERR_NOSUCHCHANNEL(channel)    PREFIX "403 " + channel + " :No such channel" POSTFIX 
// #define ERR_NOSUCHNICK(nick)    PREFIX "401 " + nick + " :No such nick" POSTFIX
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
// #define ERR_UNKNOWNMODE(nickname, channelname, mode)(std::string(": 472 ") + nickname + " " + channelname + " " + mode + " :is not a recognised channel mode" + POSTFIX)
#define ERR_UNKNOWNMODE( nickname, chName, mode )	   ( ": 472 " + nickname + " " + chName + " " + mode + " :is not a recognised channel mode" + POSTFIX )
// #define ERR_INVALIDMODEPARM(channelname, mode)(std::string(" : 696 ") + channelname + " Invalid mode parameter. " + mode + POSTFIX)
// #define ERR_INVALIDMODEPARM( chName, mode )			   ( ": 696 " + chName + " Invalid mode parameter " + mode + POSTFIX )
#define ERR_INVALIDMODEPARM(nick, channel, mode, param) (": 696 " + nick + " " + channel + " " + mode + " " + param + " :Invalid parameter for mode" + POSTFIX)




//Replays :
#define RPL_KICK(kikker, target, channel, reason)    ":" +  kikker +" KICK " + channel + " " + target + " :" + reason + POSTFIX
#define RPL_NAMREPLY(sender, channel, users)    PREFIX "353 " + sender + " = " + channel + " :" + users + POSTFIX
#define RPL_ENDOFNAMES(sender, channel)        PREFIX "366 " + sender + " " + channel + " :End of /NAMES list." POSTFIX
#define RPL_TOPIC(sender, channel, topic)PREFIX " 332 " + sender + " " + channel + " :" + topic + POSTFIX
#define RPL_PRIVMSG(sender, username, ip, target, msg)":" + sender + "!" + username + "@" + ip + " PRIVMSG " + target + " :" + msg + POSTFIX 
#define RPL_NICK(sender, nick)":" + sender + " NICK " + nick + POSTFIX
#define RPL_NOTOPIC(sender, channel)PREFIX ": 331 " + sender + " " + channel + " :No topic is set" + POSTFIX
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






// #pragma once

// #define CRLF "\r\n"

// #define ERR_NOTREGISTERED(nickName) ": 451 " + nickName + " :You have not registered" + CRLF
// #define ERR_UNKNOWNCOMMAND(nickName, command) ": 421 " + nickName + " " + command + " :Unknown command" + CRLF
// #define ERR_NEEDMOREPARAMS(nickName, command) ": 461 " + nickName + " " + command + " :Not enough parameters" + CRLF
// #define ERR_ALREADYREGISTERED(nickName) ": 462 " + nickName + " :You may not reregister" + CRLF
// #define ERR_PASSMISMATCH(nickName) ": 464 " + nickName + " :Password incorrect" + CRLF
// #define ERR_NICKNAMEINUSE(nickName) ": 433 " + nickName + " :Nickname is already in use" + CRLF
// #define ERR_ERRONEUSNICKNAME(nickName) ": 432 " + nickName + " :Erroneous nickname" + CRLF
#define ERR_ERRONEUSUSERNAME(userName) ": 432 " + userName + " :Erroneous username" + POSTFIX
// #define ERR_NOTONCHANNEL(nickName, channelName) ": 442 " + nickName + " #" + channelName + " :You're not on that channel" + CRLF
// #define ERR_NOSUCHCHANNEL(nickName, channelName) ": 403 " + nickName + " #" + channelName + " :No such channel" + CRLF
// #define ERR_NOSUCHNICK(nickName, recipient) ": 401 " + nickName + " " + recipient + " :No such nick" + CRLF
#define ERR_NORECIPIENT(nickName) PREFIX ": 411 " + nickName + " :No recipient given (PRIVMSG)" + POSTFIX
// #define ERR_NOTEXTTOSEND(nickName) ": 412 " + nickName + " :No text to send" + CRLF
// #define ERR_INVITEONLYCHAN(nickName, channelName) ": 473 " + nickName + " #" + channelName + " :Cannot join channel (+i)" + CRLF
// #define ERR_CHANOPRIVSNEEDED(nickName, channelName) ": 482 " + nickName + " #" + channelName + " :You're not channel operator" + CRLF
// #define ERR_NOSUCHSERVER(nickName, serverName) ": 402 " + nickName + " " + serverName + " :No such server" + CRLF
// #define ERR_NONICKNAMEGIVEN(nickName) ": 431 " + nickName + " :No nickname given" + CRLF
// #define ERR_CHANOPRIVSNEEDED(nickName, channelName) ": 482 " + nickName + " #" + channelName + " :You're not channel operator" + CRLF
// #define ERR_CHANNELISFULL(nickName, channelName) ": 471 " + nickName + " #" + channelName + " :Cannot join channel (+l)" + CRLF
// #define ERR_NOSUCHSERVER(nickName, serverName) ": 402 " + nickName + " " + serverName + " :No such server" + CRLF
// #define ERR_BADCHANNELKEY(nickName, channelName) ": 475 " + nickName + " #" + channelName + " :Cannot join channel (+k)" + CRLF
// #define ERR_BADCHANMASK(channelName) ": 476 " + channelName + " :Bad Channel Mask" + CRLF
// #define ERR_USERNOTINCHANNEL(nickName, new_user, channelName) ": 441 " + nickName + " " + new_user + " #" + channelName + " :They aren't on that channel" + CRLF
// #define ERR_UNKNOWNMODE(nickName, mode) ": 472 " + nickName + " " + mode + " :is unknown mode char to me" + POSTFIX
// #define ERR_USERONCHANNEL(nickName, new_user, channelName) ": 443 " + nickName + " " + new_user + " " + channelName + " :is already on channel" + CRLF



// #define RPL_WELCOME(nickName, userName, hostName, ip) ": 001 " + nickName + " :Welcome to the Internet Relay Network " + hostName + ip + CRLF
// #define RPL_NOTOPIC(nickName, hostName, channelName) ": 331 " + nickName + " #" + channelName + " :No topic is set" + CRLF
// #define RPL_TOPIC(nickName, hostName, ip, channelName, topic) ":" + hostName + ip + " " + nickName + " #" + channelName + " :" + topic + CRLF
#define RPL_CHANNELMODEIS(nickName, channelName, modes) ": 324 " + nickName + " " + channelName + " " + modes + POSTFIX
#define RPL_CREATIONTIME(nickName, channelName, time) ": 329 " + nickName + " " + channelName + " " + time + POSTFIX
#define RPL_TOPICWHOTIME(nickName, channelName, who, time) ": 333 " + nickName + " " + channelName + " " + who + " " + time + POSTFIX
// #define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + ipaddress + " JOIN " + channelname + CRLF)
// #define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " #" + channelname + " :" + clientslist + CRLF)
// #define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " #" + channelname + " :END of /NAMES list" + CRLF)
// #define RPL_INVITING(nickName, new_user, channelName) ": 341 " + nickName + " " + new_user + " " + channelName + CRLF
