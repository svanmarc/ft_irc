#include "MessageHandler.hpp"

void MessageHandler::sendErrorAlreadyRegistered(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTREGISTERED, "Error: You may not reregister");
}
void MessageHandler::sendErrorNotRegistered(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOTREGISTERED, "* :You have not registered");
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
void MessageHandler::sendErrorNickNameTooLong(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_ERRONEUSNICKNAME, "Error: Nickname is too long");
}
void MessageHandler::sendErrorNickNameInvalidCharacters(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_ERRONEUSNICKNAME, "Error: Nickname contains invalid characters");
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
void MessageHandler::sendErrorNickNameAlreadyExists(ClientHandler *clientHandler, const std::string &nickname) {
    std::string response = clientHandler->getUser().getNickname() + " " + nickname + " :Nickname is already in use";
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NICKNAMEINUSE, response);
}
void MessageHandler::sendErrorNoSuchUser(ClientHandler *clientHandler, const std::string &nickname) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK, "Error: No such user " + nickname);
}