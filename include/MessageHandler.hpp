#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "ClientHandler.hpp"
#include "IRCConstants.hpp"

#include "Channel.hpp"
#include "Irc.hpp"
#include "Server.hpp"

class MessageHandler {
private:
public:
    static void sendMessage(int socket, const std::string &message);
    static void sendResponse(ClientHandler *clientHandler, int code, const std::string &message);
    static void sendResponse(ClientHandler *clientHandler, const std::string &message);
    static void sendUserMsg(ClientHandler *clientHandler, const std::string &message, ClientHandler *sender,
                            bool notice);

    // fichier MessageError.cpp
    static void sendErrorNoAuthentification(ClientHandler *clientHandler);
    static void sendErrorAlreadyRegistered(ClientHandler *clientHandler);
    static void sendErrorNotRegistered(ClientHandler *clientHandler);
    static void sendErrorNoAuth(ClientHandler *clientHandler, const std::string &command);
    static void sendErrorNotNickNameGiven(ClientHandler *clientHandler);
    static void sendErrorNoNickNameGiven(ClientHandler *clientHandler);
    // static void sendErrorNoSuchNick(ClientHandler *clientHandler, const std::string &nickname);
    static void sendErrorNoSuchNick(ClientHandler *clientHandler);
    static void sendErrorNickNameTooLong(ClientHandler *clientHandler);
    static void sendErrorNickNameInvalidCharacters(ClientHandler *clientHandler);
    static void sendErrorNoUserParams(ClientHandler *clientHandler);
    static void sendErrorTooManyAttempts(ClientHandler *clientHandler);
    static void sendErrorIncorrectPassword(ClientHandler *clientHandler, const std::string &strAttempt);
    static void sendErrorNoPasswordGiven(ClientHandler *clientHandler);
    static void sendErrorJoinChannel(ClientHandler *clientHandler, const std::string &channelName);
    static void sendErrorUnknownCommand(ClientHandler *clientHandler);
    static void sendErrorNoTarget(ClientHandler *clientHandler);
    static void sendErrorNoMessage(ClientHandler *clientHandler);
    static void sendErrorNickNameAlreadyExists(ClientHandler *clientHandler, const std::string &nickname);
    static void sendErrorNoSuchUser(ClientHandler *clientHandler, const std::string &nickname);
    static void sendErrorNotInChannel(ClientHandler *clientHandler, const std::string &channelName);
    static void sendErrorNoPingParams(ClientHandler *clientHandler);
    static void sendErrorModeNeedMoreParams(ClientHandler *clientHandler);
    static void sendErrorModeAlreadySet(ClientHandler *clientHandler, const std::string &mode);
    static void sendErrorNoChangeModeForOther(ClientHandler *clientHandler);
    static void sendErrorUnknownMode(ClientHandler *clientHandler, const std::string &mode);
    static void sendErrorNoSuchChannel(ClientHandler *clientHandler, const std::string &channelName);
    static void sendErrorInviteOnly(ClientHandler *clientHandler, const std::string &channelName);
    static void sendErrortooManyTargets(ClientHandler *clientHandler);
    static void sendErrorAlreadyInChannel(ClientHandler *clientHandler, const std::string &nickname,
                                          const std::string &channelName);
    static void sendErrorAlreadyInvited(ClientHandler *clientHandler, const std::string &nickname,
                                        const std::string &channelName);
    static void sendErrorNotChannelOperator(ClientHandler *clientHandler);
    static void sendErrorNotEnoughParams(ClientHandler *clientHandler);
    static void sendErrorClientKicked(ClientHandler *clientHandler, const std::string &message);
    static void sendErrorKickOwner(ClientHandler *clientHandler);
    static void sendErrorBadChannelKey(ClientHandler *clientHandler, const std::string &channelName);
    static void sendErrorChannelFull(ClientHandler *clientHandler, const std::string &channelName);
    static void sendErrorBadFormatPwd(ClientHandler *clientHandler);
    static void sendErrorPwdNeeded(ClientHandler *clientHandler);
    static void sendErrorBadMode(ClientHandler *clientHandler, const std::string &mode);
    static void sendError(ClientHandler *clientHandler, int code, const std::string &channel,
                          const std::string &message);

    // fichier MessageUser.cpp
    static void sendCAP(ClientHandler *client_handler);
    static void sendNothing(std::string &cmd);
    static void sendWelcomeMessage(ClientHandler *clientHandler);
    static void sendChangeNickName(ClientHandler *clientHandler, const std::string &oldNickName,
                                   const std::string &newNickName);
    static void sendAuthentificationSuccess(ClientHandler *clientHandler);
    static void sendMessageToUser(ClientHandler *clientHandler, ClientHandler *clientTarget, const std::string &message,
                                  bool notice);
    static void sendModeChange(ClientHandler *clientHandler, const std::string &mode, const std::string &target);
    static void sendGoodbye(ClientHandler *clientHandler);

    // Fichier MessageWhois
    static void MessageWhoisUser(ClientHandler *clientHandler, ClientHandler *targetClient);
    static void MessageWhois(ClientHandler *clientHandler, ClientHandler *targetClient);
    static void MessageWhoisNotFound(ClientHandler *clientHandler, const std::string &targetNickname);
    static void MessageWhoisNoGiven(ClientHandler *clientHandler);
    static void sendPong(ClientHandler *clientHandler);

    // Fichier MessageChannel
    static void sendWelcomeToChannel(ClientHandler *clientHandler, const Channel &channel);
    static void sendNewMemberToChannel(ClientHandler *clientHandler, Channel &channel);
    static void sendMessageToAllClientsInChannel(Channel &channel, const std::string &message,
                                                 ClientHandler *clientHandler, bool sendToSender);
    static void sendCurrentMemberListToNew(ClientHandler *clientHandler, Channel &channel);
    static void sendEndOfNamesList(ClientHandler *clientHandler, Channel &channel);
    static void sendInviteNotification(ClientHandler *clientHandler, ClientHandler *targetClient, Channel &channel);
    static void sendChannelModes(ClientHandler *clientHandler, Channel &channel, const std::string &modeSign,
                                 const std::string mode);
    static void sendOpMode(ClientHandler *clientHandler, ClientHandler *targetClient, Channel &channel,
                           const std::string &modeSign);

    // Fichier MessageFormater
    static std::string messageWithServerPrefixAndSender(ClientHandler *clientHandler, std::string &originalMessage);

    static void sendMessageToClient(ClientHandler *clientHandler, const std::string &message);
};


#endif // MESSAGEHANDLER_H
