#include "MessageHandler.hpp"




void MessageHandler::MessageWhoisUser(ClientHandler *clientHandler, ClientHandler *targetClient) {
    std::string response = MessageHandler::MessageWhoisFormat(clientHandler, targetClient);
    response += std::string(targetClient->getUser().getRealname()) + " " + targetClient->getServer()->getServerName() + " ";
    response += std::string(" * :") + targetClient->getUser().getNickname();
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_WHOISUSER, response);
}


void MessageHandler::MessageWhois(ClientHandler *clientHandler, ClientHandler *targetClient) {
    std::string serverName = clientHandler->getServer()->getServerName();
    std::string templagte = ":" + serverName + " ";
    std::string nickName = targetClient->getUser().getNickname();
    std::string realname = targetClient->getUser().getRealname();
    std::string userServer = targetClient->getServer()->getServerName();
    std::string realnameTile = " ~" + realname;

    std::string w1 = templagte +  "311 " + nickName + " " +  nickName + realname + realnameTile + " " + userServer + " * :" + realname;
    std::string w3 = templagte + "312 " + nickName + " " + nickName + serverName + ": ubuntu-linux default configuration";

    // Calculer le temps d'inactivité
    std::time_t now = std::time(0);
    std::time_t signonTime = clientHandler->getUser().getConnectionTime();

    std::time_t idleTime = std::difftime(now, signonTime);
    std::ostringstream idleTimeStr;
    std::ostringstream signonTimeStr;
    idleTimeStr << static_cast<long>(idleTime);
    signonTimeStr << static_cast<long>(signonTime);

    std::string w4 = templagte + "317 " + nickName + " " + nickName + " " +
                     idleTimeStr.str() + " " + signonTimeStr.str() + " :seconds idle, signon time";
    std::string w5 = templagte + "318 " + nickName + " " + nickName + " :End of /WHOIS list.";

    std::cout << "MessageWhois " << targetClient->getUser().getNickname() << std::endl;

    std::vector<std::string> channels = targetClient->getChannels();
    std::string targetChannels = "";
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
        targetChannels += *it + " ";
    }
    //:irc.localhost 319 u u :#t #e
    std::string w6 = templagte + "319 " + nickName + " " + nickName + " :" + targetChannels;

    MessageHandler::sendMessage(clientHandler->getSocket(), w1);
    if (channels.size() > 0)
        MessageHandler::sendMessage(clientHandler->getSocket(), w6);
    MessageHandler::sendMessage(clientHandler->getSocket(), w3);
    MessageHandler::sendMessage(clientHandler->getSocket(), w4);
    MessageHandler::sendMessage(clientHandler->getSocket(), w5);

}

void MessageHandler::MessageWhoisNotFound(ClientHandler *clientHandler, const std::string &targetNickname) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NOSUCHNICK, targetNickname + " UnknownNick :No such nick/channel" );
}

void MessageHandler::MessageWhoisNoGiven(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::ERR_NONICKNAMEGIVEN, "No nickname given");
}

void MessageHandler::sendPong(ClientHandler *clientHandler) {
     std::string message = "PONG " + clientHandler->getServer()->getServerName() + " :" + clientHandler->getServer()->getServerName();
     MessageHandler::sendMessage(clientHandler->getSocket(), message);
}