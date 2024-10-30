#include "MessageHandler.hpp"

void MessageHandler::sendCAP(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_CAPLS,
                                 "CAP * NAK :" + clientHandler->getServer()->getServerName() +
                                         " multi-prefix userhost-in-names");
    sendResponse(clientHandler, IRCConstants::RPL_CAPLS, "Ready to accept commands");
}

void MessageHandler::sendNothing(std::string &cmd) { std::cout << "CMD : " + cmd + " : NO REPONSE" << std::endl; }

const char* messages[] = {
    "Don't Panic! And Welcome to our IRC Server",
    "Time is an illusion. IRC time doubly so",
    "So long, and thanks for all the messages!",
    "Welcome to IRC - Better than the Infinite Improbability Drive",
    "The Answer to IRC, the Universe and Everything is 42",
    "Beware of the Leopard... and spam bots",
    "Share and Enjoy - Sirius Cybernetics Corporation",
    "Warning: IRC may contain traces of Pan Galactic Gargle Blaster",
    "Our IRC is mostly harmless",
    "Welcome! Did you bring your towel?",
    "Powered by Babel Fish Technology",
    "This IRC server was not made by the Vogons",
    "Please do not feed the Bugblatter Beast of Traal",
    "Today's special: Tea. Almost, but not quite, entirely unlike real tea",
    "Warning: Channel names longer than 30 letters can cause the end of the universe"
};

std::string getRandomMOTD() {
    int index = std::rand() % 15;
    return std::string(messages[index]);
}


void MessageHandler::sendWelcomeMessage(ClientHandler *clientHandler) {
    // Basic welcome messages (001-004)
    std::string randomMessage = getRandomMOTD();
    MessageHandler::sendResponse(
        clientHandler, 0,
        " 001 " + clientHandler->getUser().getNickname() +
        " :" + IRCConstants::COLOR_BROWN +  "Welcome to the Internet Relay Network " + IRCConstants::COLOR_BROWN +
        clientHandler->getUser().getNickname() + "!" +
        clientHandler->getUser().getUsername() + "@localhost"
    );

    MessageHandler::sendResponse(
        clientHandler, 0,
        " 002 " + clientHandler->getUser().getNickname() +
        " :Your host is irc.localhost, running version 2.11.2p3"
    );

    MessageHandler::sendResponse(
        clientHandler, 0,
        " 003 " + clientHandler->getUser().getNickname() +
        " :This server was created Sun Oct 3 2021 at 19:41:26 UTC"
    );

    MessageHandler::sendResponse(
        clientHandler, 0,
        " 004 " + clientHandler->getUser().getNickname() +
        " :irc.localhost 2.11.2p3 aoOirw abeiIklmnoOpqrRstv"
    );

    // Server capabilities (005)
    std::string capabilities[] = {
        "RFC2812 PREFIX=(ov)@+ CHANTYPES=#&!+ MODES=3 CHANLIMIT=#&!+:21 NICKLEN=15 TOPICLEN=255 KICKLEN=255 MAXLIST=beIR:64 CHANNELLEN=50",
        "IDCHAN=!:5 CHANMODES=beIR,k,l,imnpstaqr",
        "PENALTY FNC EXCEPTS=e INVEX=I CASEMAPPING=ascii NETWORK=ExampleNet"
    };

    for (size_t i = 0; i < 3; ++i) {
        MessageHandler::sendResponse(
            clientHandler, 0,
            std::string(" 005 ") + clientHandler->getUser().getNickname() +
            " :" + capabilities[i] + " are supported by this server"
        );
    }

    // Unique ID (deterministic or random as needed)
    MessageHandler::sendResponse(
        clientHandler, 0,
        " 042 " + clientHandler->getUser().getNickname() + " :000AAAAAI your unique ID"
    );

    // Server statistics

    std::stringstream ss;
    ss << clientHandler->getServer()->getClients().size();
    std::string nbClients = ss.str();
    MessageHandler::sendResponse(
        clientHandler, 0,
        std::string(" 251 ") + clientHandler->getUser().getNickname() +
        " :" + IRCConstants::COLOR_LIGHT_BLUE + "There are " + nbClients + " users and 0 services on 1 servers" + IRCConstants::COLOR_RESET
    );

    std::stringstream ss2;
    ss2 << clientHandler->getServer()->getChannels().size();
    std::string nbChannels = ss2.str();

    MessageHandler::sendResponse(
        clientHandler, 0,
        " 254 " + clientHandler->getUser().getNickname() +
        " :" + nbChannels + " channels formed"
    );


    // MOTD Header
    MessageHandler::sendResponse(
        clientHandler, 0,
        " 375 " + clientHandler->getUser().getNickname() +
        " :- irc.localhost Message of the Day - " +  IRCConstants::COLOR_ORANGE  +randomMessage + IRCConstants::COLOR_RESET
    );

    // MOTD Content
    MessageHandler::sendResponse(
        clientHandler, 0,
        " 372 " + clientHandler->getUser().getNickname() +
        " : " + IRCConstants::COLOR_PINK  + " -                          [ Don't Panic! ]" + IRCConstants::COLOR_RESET
    );

    // MOTD End
    MessageHandler::sendResponse(
        clientHandler, 0,
        " 376 " + clientHandler->getUser().getNickname() +
        " :- "
    );
}
void MessageHandler::sendChangeNickName(ClientHandler *clientHandler, const std::string &oldNickName,
                                        const std::string &newNickName) {
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
void MessageHandler::sendMessageToUser(ClientHandler *clientHandlerSender, ClientHandler *clientTarget,
                                       const std::string &message, bool notice) {
    MessageHandler::sendUserMsg(clientTarget, message, clientHandlerSender, notice);
}


// void MessageHandler::sendModeChange(ClientHandler *clientHandler, const std::string &mode, const std::string &target)
// {
//     const std::string response = ":" + clientHandler->getUser().getNickname() + "!" +
//                                  clientHandler->getUser().getUsername() + "@" +
//                                  clientHandler->getServer()->getServerName() + " MODE " + target + " " + mode;
//     MessageHandler::sendMessage(clientHandler->getSocket(), response);
// }

void MessageHandler::sendModeChange(ClientHandler *clientHandler, const std::string &mode, const std::string &target) {
    std::string modeMessage = "MODE " + target + " " + mode;
    modeMessage = messageWithServerPrefixAndSender(clientHandler, modeMessage);
    MessageHandler::sendMessage(clientHandler->getSocket(), modeMessage);
}


void MessageHandler::sendGoodbye(ClientHandler *clientHandler) {
    MessageHandler::sendResponse(clientHandler, IRCConstants::RPL_QUIT, "Goodbye, welcome to the real world");
}
