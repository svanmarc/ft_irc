#include "MessageHandler.hpp"

void MessageHandler::sendCAP(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_CAPLS, "CAP * NAK :" + clientHandler->getServer()->getServerName() + " multi-prefix userhost-in-names");
    sendResponse(clientHandler, IRCConstants::RPL_CAPLS, "Ready to accept commands");
}
void MessageHandler::sendWelcomeMessage(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WELCOME, "Welcome to the Internet Relay Network " + clientHandler->getUser().getNickname() + "!" + clientHandler->getUser().getUsername() + "@" + clientHandler->getUser().getHostname());
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_YOURHOST, "Your host is " + clientHandler->getServer()->getServerName() + ", running version 1.0");
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_MYINFO, clientHandler->getServer()->getServerName() + " 1.0 aoOirw abeiIklmnoOpqQrRstv");
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
