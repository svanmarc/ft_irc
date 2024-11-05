#include "MessageHandler.hpp"

//-- IN USE
// --- MESSAGE POUR LESERREURS NICK
void MessageHandler::sendErrorNickNameInvalidCharacters(ClientHandler *clientHandler) {
    std::string errorMsg = "Error: "+ IRCConstants::COLOR_RED + "Nickname contains invalid characters"
                           + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_ERRONEUSNICKNAME,
                                 errorMsg);
}
void MessageHandler::sendErrorNoNickNameGiven(ClientHandler *clientHandler) {
    std::string errorMsg = "Error: "+ IRCConstants::COLOR_RED + "No nickname given"
                       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NONICKNAMEGIVEN,
                                 errorMsg);
}

void MessageHandler::sendErrorNickNameAlreadyExists(ClientHandler *clientHandler, const std::string &nickname) {
    std::string response = clientHandler->getUser().getNickname() + " " + nickname + " :"
                           + IRCConstants::COLOR_RED + "Nickname is already in use"
                           + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NICKNAMEINUSE, response);
}

void MessageHandler::sendErrorNoSuchChannel(ClientHandler *clientHandler, const std::string &channelName) {
    //:*.42irc.net 403 tamm2 #ttttt :No such channel
    std::string response = clientHandler->getNickname() + " " + channelName +
                           "Error: " + IRCConstants::COLOR_RED + "No such channel/user "
                           + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK,
                                 response);
}

void MessageHandler::sendErrorNotInChannel(ClientHandler *clientHandler, const std::string &channelName) {
    std::string message = "Error: " + IRCConstants::COLOR_RED + "You're not in channel " + channelName
                           + IRCConstants::COLOR_RESET;
    sendResponse(clientHandler, IRCConstants::ERR_NOTINCHANNEL, message);
}

void MessageHandler::sendErrorNotChannelOperator(ClientHandler *clientHandler) {
    std::string message = "Error: " + IRCConstants::COLOR_RED + "You're not a channel operator "
                       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_CHANOPRIVSNEEDED,
                                message);
}
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

void MessageHandler::sendErrorJoinChannel(ClientHandler *clientHandler, const std::string &channelName) {
    std::string message = "Error: " + IRCConstants::COLOR_RED + "joining channel " + channelName
                       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNCOMMAND,
                                 message);
}

void MessageHandler::sendErrorInviteOnly(ClientHandler *clientHandler, const std::string &channelName) {
    //:*.42irc.net 473 moi #t :Cannot join channel (+i)
    std::string message = clientHandler->getNickname() + " " + channelName + " : "
                          + IRCConstants::COLOR_RED + "Cannot join channel (+i) "
                          + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_INVITEONLYCHAN, message);
}

void MessageHandler::sendErrorBadChannelKey(ClientHandler *clientHandler, const std::string &channelName) {
    std::string message =
            clientHandler->getUser().getNickname() + " " + channelName
            + " : "
            + IRCConstants::COLOR_RED + "Cannot join channel (Bad channel key) "
            + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_BADCHANNELKEY, message);
}

void MessageHandler::sendErrorChannelFull(ClientHandler *clientHandler, const std::string &channelName) {
    std::string message =
        clientHandler->getUser().getNickname() + " " + channelName
        + " : "
        + IRCConstants::COLOR_RED + "Cannot join channel (Channel is full) "
        + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_CHANNELISFULL, message);
}

void MessageHandler::sendErrorNoTarget(ClientHandler *clientHandler) {
    std::string message = "Error: " + IRCConstants::COLOR_RED + "No target given "
                   + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNCOMMAND, message);
}
void MessageHandler::sendErrorNoSuchNick(ClientHandler *clientHandler, const std::string &nickname) {
    std::string message = "Error: " + nickname + IRCConstants::COLOR_RED + "No such nick/channel "
                   + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK,
                                 message);
}
void MessageHandler::sendErrorKickOwner(ClientHandler *clientHandler) {
std::string message = "Error: " +  IRCConstants::COLOR_RED + " You can't kick the owner of the channel "
       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_CHANOWNPRIVNEEDED,
                                 message);
}

void MessageHandler::sendErrorNoPingParams(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "Error: No servername given");
}


void MessageHandler::sendErrorBadMode(ClientHandler *clientHandler, const std::string &mode) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNMODE, "Error: Bad mode " + mode);
}
//mode error
void MessageHandler::sendErrorModeWithMessage(ClientHandler *clientHandler, const std::string &message,
                                              const std::string &sign, Channel &channel, const std::string &param) {
    std::string errorMsg = "MODE " + channel.getName() + " " + sign + " ";
    if (!param.empty()) {
        errorMsg += param + " ";
    }
    errorMsg += ":"+ IRCConstants::COLOR_RED + message + IRCConstants::COLOR_RESET;
    MessageHandler::sendMessageToClient(clientHandler, errorMsg);
}

void MessageHandler::sendErrorNoChangeModeForOther(ClientHandler *clientHandler) {
    //:*.42irc.net 502 tamm2 :Can't change mode for other users
    std::string message = clientHandler->getNickname() + "Error: "
                          +  IRCConstants::COLOR_RED + " Can't change mode for other users "
       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTFOROTHERS,
                                 message);
}

void MessageHandler::sendErrorModeParams(ClientHandler *clientHandler) {
    std::string message = "Error: " +  IRCConstants::COLOR_RED + " Not enough parameters "
       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, message);
}
void MessageHandler::sendErrorNoMessage(ClientHandler *clientHandler) {
    std::string message = "Error: " +  IRCConstants::COLOR_RED + " No message specified "
       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, message);
}
void MessageHandler::sendErrorAlreadyRegistered(ClientHandler *clientHandler) {
    std::string message = "Error: " +  IRCConstants::COLOR_RED + " You may not reregister "
       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTREGISTERED, message);
}

void MessageHandler::sendErrorNoPasswordGiven(ClientHandler *clientHandler) {
    std::string message = "Error: " +  IRCConstants::COLOR_RED + " YNo password given "
       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_PASSWDMISMATCH, message);
}
void MessageHandler::sendErrorTooManyAttempts(ClientHandler *clientHandler) {
    std::string message = "Error: " +  IRCConstants::COLOR_RED + " Too many attempts, Disconnecting... "
       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_TOOMANYATTEMPTS,
                                 message);
}

void MessageHandler::sendErrorIncorrectPassword(ClientHandler *clientHandler, const std::string &strAttempt) {
    std::string message = "Error: " +  IRCConstants::COLOR_RED + " Password incorrect, " + strAttempt + " attempts left "
       + IRCConstants::COLOR_RESET;
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_PASSWDMISMATCH,
                                 message);
}

// OHTER
void MessageHandler::sendErrorNoAuthentification(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_PASSWDMISMATCH,
                                 "Error: You need to authenticate first");
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
void MessageHandler::sendErrorNickNameTooLong(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_ERRONEUSNICKNAME, "Error: Nickname is too long");
}

void MessageHandler::sendErrorNoUserParams(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS,
                                 "Error: Manque des paramètres : USER username hostname servername :realname");
}



void MessageHandler::sendErrorUnknownCommand(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNCOMMAND, "Error: Unknown command");
}




void MessageHandler::sendErrorNoSuchUser(ClientHandler *clientHandler, const std::string &nickname) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK, "Error: No such user " + nickname);
}






void MessageHandler::sendErrorModeNeedMoreParams(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "Error: Not enough parameters");
}



void MessageHandler::sendErrorModeAlreadySet(ClientHandler *clientHandler, const std::string &mode) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNMODE,
                                 "Error: Mode " + mode + " is already set");
}
void MessageHandler::sendErrorUnknownMode(ClientHandler *clientHandler, const std::string &mode) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNMODE, "Error: Unknown mode " + mode);
}



void MessageHandler::sendErrortooManyTargets(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_TOOMANYATTEMPTS, "Error: Too many targets");
}







void MessageHandler::sendErrorNotEnoughParams(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "Error: Not enough parameters");
}

void MessageHandler::sendErrorClientKicked(ClientHandler *clientHandler, const std::string &message) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_KICKEDFROMCHAN, message);
}



void MessageHandler::sendErrorCantTakeOp(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_CHANOWNPRIVNEEDED,
                                 "Error: You can't take operator status from the owner of the channel");
}



void MessageHandler::sendErrorBadFormatPwd(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_PASSWDMISMATCH, "Error: Bad password format");
}

void MessageHandler::sendErrorPwdNeeded(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_PASSWDMISMATCH, "Error: Password needed");
}


void MessageHandler::sendError(ClientHandler *clientHandler, int code, const std::string &channel,
                               const std::string &message) {
    std::ostringstream response;
    response << ":" << clientHandler->getServer()->getServerName() << " " << code << " " << clientHandler->getNickname()
             << " " << channel << " :" << message;
    sendMessage(clientHandler->getSocket(), response.str());
}
