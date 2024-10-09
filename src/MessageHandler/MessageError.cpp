#include "MessageHandler.hpp"

void MessageHandler::sendErrorAlreadyRegistered(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTREGISTERED, "Error: You may not reregister");
}
void MessageHandler::sendErrorNotRegistered(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTREGISTERED, "Error: You have not registered");
}

void MessageHandler::sendErrorNotNickNameGiven(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NONICKNAMEGIVEN, "Error: No nickname given");
}
void MessageHandler::sendErrorNoNickNameGiven(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NONICKNAMEGIVEN, "Error: No nickname given");
}
void MessageHandler::sendErrorNoSuchNick(ClientHandler *clientHandler, const std::string &nickname) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK, "Error: "+ nickname + " :No such nick/channel");
}
void MessageHandler::sendErrorNoUserParams(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "Error: Manque des paramètres : USER username hostname servername :realname");
}
void MessageHandler::sendErrorTooManyAttempts(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_TOOMANYATTEMPTS, "Error: Too many attempts, Disconnecting...");
}
void MessageHandler::sendErrorIncorrectPassword(ClientHandler *clientHandler, const std::string &strAttempt) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_PASSWDMISMATCH, "Error: Password incorrect, " + strAttempt + " attempts left");
}
void MessageHandler::sendErrorNoPasswordGiven(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOPASSWORDGIVEN, "Error: No password given");
}
void MessageHandler::sendErrorUnknownCommand(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNCOMMAND, "Error: Unknown command");
}
void MessageHandler::sendErrorJoinChannel(ClientHandler *clientHandler, const std::string &channelName) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNCOMMAND, "Error joining channel " + channelName);
}
void MessageHandler::sendErrorNoTarget(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_UNKNOWNCOMMAND, "Error: No target given");
}
void MessageHandler::sendErrorNoMessage(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "Error: No message specified");
}