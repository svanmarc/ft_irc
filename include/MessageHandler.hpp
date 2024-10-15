#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "ClientHandler.hpp"
#include "IRCConstants.hpp"
#include "Irc.hpp"
#include "MessageHandler.hpp"
#include "Server.hpp"

class MessageHandler {
private:
public:
    static const int RPL_WELCOME;
    static const int RPL_YOURHOST;
    static const int RPL_MYINFO;
    static const int RPL_NICK;
    static const int RPL_NICKCHANGE;
    static const int RPL_WELCOMECHANNEL;
    static const int RPL_LISTSTART;
    static void sendMessage(int socket, const std::string &message);
    static void sendResponse(ClientHandler *clientHandler, int code, const std::string &message);
    static void sendResponse(ClientHandler *clientHandler, const std::string &message);
    static void sendUserMsg(ClientHandler *clientHandler, const std::string &message, const std::string &target);

    // fichier MessageError.cpp
    static void sendErrorAlreadyRegistered(ClientHandler *clientHandler);
    static void sendErrorNotRegistered(ClientHandler *clientHandler);
    static void sendErrorNotNickNameGiven(ClientHandler *clientHandler);
    static void sendErrorNoNickNameGiven(ClientHandler *clientHandler);
    static void sendErrorNoSuchNick(ClientHandler *clientHandler, const std::string &nickname);
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

    // fichier MessageUser.cpp
    static void sendCAP(ClientHandler *client_handler);
    static void sendWelcomeMessage(ClientHandler *clientHandler);
    static void sendChangeNickName(ClientHandler *clientHandler, const std::string &oldNickName,
                                   const std::string &newNickName);
    static void sendWelcomeToChannel(ClientHandler *clientHandler, const std::string &channelName);
    static void sendAuthentificationSuccess(ClientHandler *clientHandler);
    static void sendMessageToUser(ClientHandler *clientHandler, ClientHandler *clientTarget,
                                  const std::string &message);
    static void sendGoodbye(ClientHandler *clientHandler);

    // Fichier MessageWhois
    static void MessageWhoisUser(ClientHandler *clientHandler, ClientHandler *targetClient);
    static void MessageWhois(ClientHandler *clientHandler, ClientHandler *targetClient);
    static void MessageWhoisNotFound(ClientHandler *clientHandler, const std::string &targetNickname);
    static void MessageWhoisNoGiven(ClientHandler *clientHandler);
};


#endif // MESSAGEHANDLER_H
