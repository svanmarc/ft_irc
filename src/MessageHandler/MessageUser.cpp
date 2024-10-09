#include "MessageHandler.hpp"

void MessageHandler::sendCAP(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_CAPLS, "CAP * NAK :" + clientHandler->getServer()->getServerName() + " No capabilities enabled");
    sendResponse(clientHandler, IRCConstants::RPL_CAPLS, "Ready to accept commands");
}
void MessageHandler::sendWelcomeMessage(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WELCOME, "Welcome to the Internet Relay Network " + clientHandler->getUser().getNickname() + "!" + clientHandler->getUser().getUsername() + "@" + clientHandler->getUser().getHostname());
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_YOURHOST, "Your host is " + clientHandler->getServer()->getServerName() + ", running version 1.0");
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_MYINFO, clientHandler->getServer()->getServerName() + " 1.0 aoOirw abeiIklmnoOpqQrRstv");
}
void MessageHandler::sendNewNickName(ClientHandler *clientHandler, const std::string &newNickName) {
    std::string response = ":";
    response += clientHandler->getServer()->getServerName();
    response += " NICK ";
    response += newNickName;
    MessageHandler::sendResponse(clientHandler, response);
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
void MessageHandler::sendWelcomeToChannel(ClientHandler *clientHandler, const std::string &channelName) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WELCOME, "Welcome to " + channelName);
}
void MessageHandler::sendAuthentificationSuccess(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, "Authentification success");
}
void MessageHandler::sendMessageToUser(ClientHandler *clientHandler, ClientHandler *clientTarget, const std::string &message) {
    const std::string response = ":" + clientHandler->getNickname() + " PRIVMSG " + clientTarget->getNickname() + " :" + message;
    MessageHandler::sendResponse(clientTarget, message);
}

void MessageHandler::sendGoodbye(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_QUIT, "Goodbye, welcome to the real world");
}
