#ifndef IRCCONSTANTS_H
#define IRCCONSTANTS_H

#include <string>


class IRCConstants {
public:

    //colors
    static const std::string COLOR_WHITE;
    static const std::string COLOR_BLACK;
    static const std::string COLOR_BLUE;
    static const std::string COLOR_GREEN;
    static const std::string COLOR_RED;
    static const std::string COLOR_BROWN;
    static const std::string COLOR_PURPLE;
    static const std::string COLOR_ORANGE;
    static const std::string COLOR_YELLOW;
    static const std::string COLOR_LIGHT_GREEN;
    static const std::string COLOR_TEAL;
    static const std::string COLOR_LIGHT_BLUE;
    static const std::string COLOR_PINK;
    static const std::string COLOR_GREY;
    static const std::string COLOR_RESET;
    //irc commands
    static const int RPL_WELCOME = 001; // 001: Welcome to the Internet Relay Network
    static const int RPL_YOURHOST = 002; // 002: Your host is <server>, running version <version>
    static const int RPL_CREATED = 003; // 003: This server was created <date>
    static const int RPL_MYINFO = 004; // 004: <server> <version> :<options>
    static const int RPL_BOUNCE = 005; // 005: Try server <server>, port <port>
    static const int RPL_USERHOST = 302; // 302: :*1
    static const int RPL_ISON = 303; // 303: :<nick>[ <nick>]
    static const int RPL_AWAY = 301; // 301: <nick> :<away>
    static const int RPL_UNAWAY = 305; // 305: You are no longer marked as being away
    static const int RPL_NOWAWAY = 306; // 306: You have been marked as being away
    static const int RPL_WHOISUSER = 311; // 311: <nick> <user> <host> * :<real name>
    static const int RPL_WHOISSERVER = 378; // 312: <nick> <server> :<server info>
    static const int RPL_WHOISOPERATOR = 313; // 313: <nick> :is an IRC operator
    static const int RPL_WHOISIDLE = 317; // 317: <nick> <seconds> :seconds idle
    static const int RPL_ENDOFWHOIS = 318; // 318: End of WHOIS list
    static const int RPL_WHOISCHANNELS = 319; // 319: <nick> :{[@|+]<channel><space>}
    static const int RPL_WHO = 352; // 352: <channel> <user> <host> <server> <nick> <H|G>[*] :<hop count> <real name>
    static const int RPL_LISTSTART = 321; // 321: Channel :Users  Name
    static const int RPL_LIST = 322; // 322: <channel> <# visible> :<topic>
    static const int RPL_LISTEND = 323; // 323: End of LIST
    static const int RPL_CHANNELMODEIS = 324; // 324: <channel> <mode> <mode params>
    static const int RPL_NOTOPIC = 331; // 331: <channel> :No topic is set
    static const int RPL_TOPIC = 332; // 332: <channel> :<topic>
    static const int RPL_INVITING = 341; // 341: <channel> <nick> :Inviting <nick> to <channel>
    static const int RPL_SUMMONING = 342; // 342: <nick> :Summoning user
    static const int RPL_VERSION = 351; // 351: <version> <server> :<comments>
    static const int RPL_WHOREPLY = 352; // 352: <channel> <user> <host> <server> <nick> <H|G>* :<hop count> <real name>
    static const int RPL_NAMREPLY = 353; // 353: <channel> :<nick> <nick> <nick>
    static const int RPL_ENDOFWHO = 315; // 315: End of WHO list
    static const int RPL_ENDOFNAMES = 366; // 366: End of NAMES list
    static const int RPL_MOTD = 372; // 372: :<text>
    static const int RPL_CAPLS = 375;
    static const int RPL_ENDOFMOTD = 376; // 376: End of MOTD command
    static const int RPL_QUIT = 0; // 0: Quit


    // Erreurs
    static const int ERR_UNKNOWNCOMMAND = 421; // 421: Unknown command
    static const int ERR_NICKNAMEINUSE = 433; // 433: Nickname is already in use
    static const int ERR_ERRONEUSNICKNAME = 432; // 432: Erroneous nickname
    static const int ERR_NONICKNAMEGIVEN = 431; // 431: No nickname given
    static const int ERR_NEEDMOREPARAMS = 461; // 461: Not enough parameters
    static const int ERR_ALREADYREGISTERED = 462; // 462: You may not reregister
    static const int ERR_PASSWDMISMATCH = 464; // 464: Password incorrect or not given
    static const int ERR_TOOMANYATTEMPTS = 465; // 465: Too many attempts
    static const int ERR_NOSUCHNICK = 401; // 401: No such nick/channel
    static const int ERR_NOTREGISTERED = 451; // 451: You have not registered
    static const int ERR_CHANNELISFULL = 471; // 471: Cannot join channel (+l)
    static const int ERR_UNKNOWNMODE = 472; // 472: Unknown mode
    static const int ERR_INVITEONLYCHAN = 473; // 473: Cannot join channel (+i)
    static const int ERR_NOTINCHANNEL = 442; // 442: You're not on that channel
    static const int ERR_NOTFOROTHERS = 502; // 502: Can't change mode for other users
    static const int ERR_USERONCHANNEL = 443; // 443: is already on channel
    static const int ERR_CHANOPRIVSNEEDED = 482; // 482: You're not channel operator
    static const int ERR_KICKEDFROMCHAN = 470; // 470: You have been kicked from the channel
    static const int ERR_CHANOWNPRIVNEEDED = 499; // 499: You're not channel owner
    static const int ERR_BADCHANNELKEY = 475; // 475: Cannot join channel (+k)
};

// from https://www.alien.net.au/irc/irc2numerics.html


#endif // IRCCONSTANTS_H
