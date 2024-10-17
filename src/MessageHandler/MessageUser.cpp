#include "MessageHandler.hpp"

void MessageHandler::sendCAP(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_CAPLS, "CAP * NAK :" + clientHandler->getServer()->getServerName() + " multi-prefix userhost-in-names");
    sendResponse(clientHandler, IRCConstants::RPL_CAPLS, "Ready to accept commands");
}

void MessageHandler::sendNothing(std::string &cmd) {
    std::cout << "CMD : "+ cmd + " : NO REPONSE" << std::endl;
}

void MessageHandler::sendWelcomeMessage(ClientHandler *clientHandler) {
    //:*.localhost 001 client2 :Welcome to the Internet Relay Network client2!root@127.0.0.1.hosted-by-42lausanne.ch
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WELCOME, clientHandler->getUser().getNickname() + " :Welcome to the Internet Relay Network " + clientHandler->getUser().getNickname() + "!" + clientHandler->getUser().getUsername() + "@" + clientHandler->getUser().getHostname());
    //:*.localhost 002 client2 :Your host is *.localhost, running version 1.0
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_YOURHOST, clientHandler->getUser().getNickname() + " :Your host is " + clientHandler->getServer()->getServerName() + ", running version 1.0");
    //:*.42irc.net 003 client2 :This server was created Thu Oct 17 14:16:27 2024
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_CREATED, clientHandler->getUser().getNickname() + " :This server was created This server was created Thu Oct 17 14:16:27 2024");
    //:*.localhost 004 client2 :*.42irc.net 1.0 Channel modes +ntikl
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_MYINFO, clientHandler->getUser().getNickname() + " :"+ clientHandler->getServer()->getServerName() + " 1.0 Channel modes +ntikl");
    sendCAP(clientHandler);
}
void MessageHandler::sendChangeNickName(ClientHandler *clientHandler, const std::string &oldNickName, const std::string &newNickName) {
    std::string response = ":";
    response += oldNickName;
    response += "!";
    response += newNickName;
    response += "@";
    response += clientHandler->getServer()->getServerName();
    response += " NICK :";
    response += newNickName;
    MessageHandler::sendResponse(clientHandler, response);
}

void MessageHandler::sendAuthentificationSuccess(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, "Authentification success");
}
void MessageHandler::sendMessageToUser(ClientHandler *clientHandlerSender, ClientHandler *clientTarget, const std::string &message) {
    MessageHandler::sendUserMsg(clientTarget, message, clientHandlerSender->getUser().getNickname());
}

void MessageHandler::sendGoodbye(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_QUIT, "Goodbye, welcome to the real world");
}
