#include "CommandHandler.hpp"

CommandHandler::CommandHandler(Server &server) : m_server(server) {}

void CommandHandler::handleCommandNoRegistred(const std::string &command, ClientHandler *clientHandler) {
    try {
        std::cout << "Received command: " << command << std::endl;
        std::string cmd = parseCommand(command);
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);
        if (cmd == "CAP") {
            handleCap(clientHandler, command);
        } else if (cmd == "NICK") {
            handleNick(command, clientHandler);
        } else if (cmd == "USER" || cmd == "USERHOST") {
            handleUser(command, clientHandler);
        } else if (cmd == "PASS") {
            handlePass(command, clientHandler);
        } else if (cmd == "WHOIS") {
            handleWhois(command, clientHandler);
        } else {
            MessageHandler::sendErrorNotRegistered(clientHandler);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error handling command: " << e.what() << std::endl;
        MessageHandler::sendErrorUnknownCommand(clientHandler);
    }
}

void CommandHandler::handleCommand(const std::string &command, ClientHandler *clientHandler) {
    try {
        std::cout << "Received command: " << command << std::endl;
        std::string cmd = parseCommand(command);
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);
        if (cmd == "CAP") {
            handleCap(clientHandler, command);
        } else if (cmd == "NICK") {
            handleNick(command, clientHandler);
        } else if (cmd == "USER" || cmd == "USERHOST") {
            handleUser(command, clientHandler);
        } else if (cmd == "PASS") {
            handlePass(command, clientHandler);
        } else if (cmd == "MODE") {
            handleMode(command, clientHandler);
        } else if (cmd == "WHOIS") {
            handleWhois(command, clientHandler);
        } else if (cmd == "JOIN") {
            std::cout << "JOIN command received" << std::endl;
            const std::string channelName = command.substr(5);
            const bool joinStatus = clientHandler->joinChannel(channelName);
            if (joinStatus) {
                MessageHandler::sendWelcomeToChannel(clientHandler, channelName);
            } else {
                MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
            }
        } else if (cmd == "QUIT") {
            handleQuit(clientHandler);
        } else if (cmd == "PRIVMSG") {
            handlePrivMsg(command, clientHandler);
        } else {
            MessageHandler::sendErrorUnknownCommand(clientHandler);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error handling command: " << e.what() << std::endl;
        MessageHandler::sendErrorUnknownCommand(clientHandler);
    }
}


void CommandHandler::handleCap(ClientHandler *clientHandler, const std::string &command) {
    if (command.find("LS") != std::string::npos) {
        MessageHandler::sendCAP(clientHandler);
        std::cout << "CAP * LS :multi-prefix" << std::endl;
    }
}


void CommandHandler::handleMode(const std::string &command, ClientHandler *clientHandler) {
    //------------ A REFLECHIR ------------
    std::string mode;
    size_t pos = command.find("MODE");
    if (pos != std::string::npos && pos + 5 < command.length()) {
        mode = trim(command.substr(pos + 5));
    }

    clientHandler->getUser().setUserMode(mode);

    std::string response = clientHandler->getNickname();
    response += " +";
    response += mode;
    MessageHandler::sendResponse(clientHandler, RPL_CHANNELMODEIS, response);
}

void CommandHandler::handleQuit(ClientHandler *clientHandler) {
    MessageHandler::sendGoodbye(clientHandler);
    std::cout << "Client " << clientHandler->getNickname() << " has quit." << std::endl;
    m_server.handleClientDisconnect(clientHandler->getSocket());
}
