#include "MessageHandler.hpp"


void MessageHandler::MessageWhoisUser(ClientHandler *clientHandler, ClientHandler *targetClient) {
    std::string response = std::string(clientHandler->getNickname() + " ");
    response += std::string(targetClient->getNickname() + " ");
    response += std::string(targetClient->getUser().getUsername() + " ");
    response += std::string(targetClient->getUser().getHostname() + " * :");
    response += std::string(targetClient->getUser().getRealname());
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WHOISUSER, response);

}

void MessageHandler::MessageWhois(ClientHandler *clientHandler, ClientHandler *targetClient) {
    MessageWhoisUser(clientHandler, targetClient);
    std::string const repWhoisserver = std::string(targetClient->getNickname() + " localhost :Server description");
    std::string const repWhoisChannels = std::string(targetClient->getNickname() + " :{[@|+]<channel><space>}");
    std::string const repWhoisIdl = std::string(targetClient->getNickname() + " 0 0 :seconds idle, signon time");
    std::string const repWhoisEnd = std::string(targetClient->getNickname() + " :End of /WHOIS list");


    MessageWhoisUser(clientHandler, targetClient);
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WHOISSERVER, repWhoisserver);
    //A faire le calcul du temps de connexion
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WHOISIDLE, repWhoisIdl);
    // A FAIRE quand on onra fait les channels
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WHOISCHANNELS, repWhoisChannels);
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_ENDOFWHOIS, repWhoisEnd);
}

void MessageHandler::MessageWhoisNotFound(ClientHandler *clientHandler, const std::string &targetNickname) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK, targetNickname + " UnknownNick :No such nick/channel" );
}

void MessageHandler::MessageWhoisNoGiven(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NONICKNAMEGIVEN, "No nickname given");
}

void MessageHandler::sendPong(ClientHandler *clientHandler) {
<<<<<<< Updated upstream
    std::string message = "PONG " + clientHandler->getServer()->getServerName() + " :" + clientHandler->getServer()->getServerName();
    MessageHandler::sendMessage(clientHandler->getSocket(), message);
=======
     std::string message = "PONG " + clientHandler->getServer()->getServerName() + " :" + clientHandler->getServer()->getServerName();
     MessageHandler::sendMessage(clientHandler->getSocket(), message);
}

void MessageHandler::sendErrorNoPingParams(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NEEDMOREPARAMS, "PING :Not enough parameters");
>>>>>>> Stashed changes
}