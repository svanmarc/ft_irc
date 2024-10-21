#ifndef IRCCONSTANTS_H
#define IRCCONSTANTS_H

class IRCConstants {
public:
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
    static const int RPL_WHOISSERVER = 312; // 312: <nick> <server> :<server info>
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
    static const int ERR_NONICKNAMEGIVEN = 433; // 433: No nickname given
    static const int ERR_NEEDMOREPARAMS = 461; // 461: Not enough parameters
    static const int ERR_ALREADYREGISTRED = 462; // 462: You may not reregister
    static const int ERR_PASSWDMISMATCH = 464; // 464: Password incorrect
    static const int ERR_NOPASSWORDGIVEN = 464; // 464: No password given
    static const int ERR_TOOMANYATTEMPTS = 464; // 464: Too many attempts
    static const int ERR_NOSUCHNICK = 401; // 401: No such nick/channel
    static const int ERR_NOTREGISTERED = 451; // 451: You have not registered
    static const int ERR_CHANNELISFULL = 471; // 471: Cannot join channel (+l)
    static const int ERR_UNKNOWNMODE = 472; // 472: Unknown mode
    static const int ERR_INVITEONLYCHAN = 473; // 473: Cannot join channel (+i)
    static const int ERR_BANNEDFROMCHAN = 474; // 474: Cannot join channel (+b)
    static const int ERR_NOTINCHANNEL = 442; // 442: You're not on that channel
};

// from https://www.alien.net.au/irc/irc2numerics.html


#endif // IRCCONSTANTS_H
