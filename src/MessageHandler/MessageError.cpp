#include "MessageHandler.hpp"

void MessageHandler::sendErrorNoAuthentification(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_PASSWDMISMATCH,
                                 "Error: You need to authenticate first");
}
void MessageHandler::sendErrorAlreadyRegistered(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTREGISTERED, "Error: You may not reregister");
}
void MessageHandler::sendErrorNotRegistered(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTREGISTERED, "* :You have not registered");
}
void MessageHandler::sendErrorNoAuth(ClientHandler *clientHandler, const std::string &command) {
    // exemple :*.localhost 451 * JOIN :You have not registered
    std::string response = " * " + command + " :You have not registered";
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTREGISTERED, response);
}
void MessageHandler::sendErrorNotNickNameGiven(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NONICKNAMEGIVEN, "Error: No nickname given");
}
void MessageHandler::sendErrorNoNickNameGiven(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NONICKNAMEGIVEN, "Error: No nickname given");
}
void MessageHandler::sendErrorNoSuchNick(ClientHandler *clientHandler, const std::string &nickname) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK,
                                 "Error: " + nickname + " :No such nick/channel");
}
void MessageHandler::sendErrorNickNameTooLong(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_ERRONEUSNICKNAME, "Error: Nickname is too long");
}
void MessageHandler::sendErrorNickNameInvalidCharacters(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_ERRONEUSNICKNAME,
                                 "Error: Nickname contains invalid characters");
}
void MessageHandler::sendErrorNoUserParams(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS,
                                 "Error: Manque des paramètres : USER username hostname servername :realname");
}
void MessageHandler::sendErrorTooManyAttempts(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_TOOMANYATTEMPTS,
                                 "Error: Too many attempts, Disconnecting...");
}
void MessageHandler::sendErrorIncorrectPassword(ClientHandler *clientHandler, const std::string &strAttempt) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_PASSWDMISMATCH,
                                 "Error: Password incorrect, " + strAttempt + " attempts left");
}
void MessageHandler::sendErrorNoPasswordGiven(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_PASSWDMISMATCH, "Error: No password given");
}
void MessageHandler::sendErrorUnknownCommand(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNCOMMAND, "Error: Unknown command");
}
void MessageHandler::sendErrorJoinChannel(ClientHandler *clientHandler, const std::string &channelName) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNCOMMAND,
                                 "Error joining channel " + channelName);
}
void MessageHandler::sendErrorNoTarget(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNCOMMAND, "Error: No target given");
}
void MessageHandler::sendErrorNoMessage(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "Error: No message specified");
}
void MessageHandler::sendErrorNickNameAlreadyExists(ClientHandler *clientHandler, const std::string &nickname) {
    std::string response = clientHandler->getUser().getNickname() + " " + nickname + " :Nickname is already in use";
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NICKNAMEINUSE, response);
}
void MessageHandler::sendErrorNoSuchUser(ClientHandler *clientHandler, const std::string &nickname) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK, "Error: No such user " + nickname);
}

void MessageHandler::sendErrorNotInChannel(ClientHandler *clientHandler, const std::string &channelName) {
    std::string message = "You're not in channel " + channelName;
    sendResponse(clientHandler, IRCConstants::ERR_NOTINCHANNEL, message);
}

// void MessageHandler::sendErrorNoPingParams(ClientHandler *clientHandler) {
//     MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "Error: No servername given");
// }

void MessageHandler::sendErrorNoSuchChannel(ClientHandler *clientHandler, const std::string &channelName) {
    //:*.42irc.net 403 tamm2 #ttttt :No such channel
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK,
                                 clientHandler->getNickname() + " " + channelName + " :No such channel/user ");
}
void MessageHandler::sendErrorNoChangeModeForOther(ClientHandler *clientHandler) {
    //:*.42irc.net 502 tamm2 :Can't change mode for other users
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTFOROTHERS,
                                 clientHandler->getNickname() + " :Can't change mode for other users");
}
void MessageHandler::sendErrorModeParams(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "Error: Not enough parameters");
}
void MessageHandler::sendErrorModeAlreadySet(ClientHandler *clientHandler, const std::string &mode) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNMODE,
                                 "Error: Mode " + mode + " is already set");
}
void MessageHandler::sendErrorUnknownMode(ClientHandler *clientHandler, const std::string &mode) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNMODE, "Error: Unknown mode " + mode);
}

void MessageHandler::sendErrorInviteOnly(ClientHandler *clientHandler, const std::string &channelName) {
    //:*.42irc.net 473 moi #t :Cannot join channel (+i)
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_INVITEONLYCHAN,
                                 clientHandler->getNickname() + " " + channelName + " :Cannot join channel (+i)");
}

void MessageHandler::sendErrortooManyTargets(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_TOOMANYATTEMPTS, "Error: Too many targets");
}

// void MessageHandler::sendErrorAlreadyInChannel(ClientHandler *clientHandler, const std::string &nickname,
//                                                const std::string &channelName) {
//     //:*.42irc.net 443 tamm2 #t :is already on channel
//     MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_USERONCHANNEL,
//                                  clientHandler->getNickname() + " " + channelName + " :is already on channel");
// }

// void MessageHandler::sendErrorAlreadyInvited(ClientHandler *clientHandler, const std::string &nickname,
//                                              const std::string &channelName) {
//     //:*.42irc.net 443 tamm2 #t :is already on channel
//     MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_USERONCHANNEL,
//                                  clientHandler->getNickname() + " " + channelName + " :is already on channel");
// }

void MessageHandler::sendErrorAlreadyInChannel(ClientHandler *clientHandler, const std::string &nickname,
                                               const std::string &channelName) {
    std::string message = "User " + nickname + " is already in channel " + channelName;
    sendResponse(clientHandler, IRCConstants::ERR_USERONCHANNEL, message);
}

void MessageHandler::sendErrorAlreadyInvited(ClientHandler *clientHandler, const std::string &nickname,
                                             const std::string &channelName) {
    std::string message = "User " + nickname + " is already invited to channel " + channelName;
    sendResponse(clientHandler, IRCConstants::ERR_USERONCHANNEL, message);
}

void MessageHandler::sendErrorNotChannelOperator(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_CHANOPRIVSNEEDED,
                                 "Error: You're not a channel operator");
}

void MessageHandler::sendErrorNotEnoughParams(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "Error: Not enough parameters");
}

void MessageHandler::sendErrorClientKicked(ClientHandler *clientHandler, const std::string &message) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_KICKEDFROMCHAN, message);
}
