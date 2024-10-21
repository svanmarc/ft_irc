#include "CommandHandler.hpp"

void CommandHandler::handleQuit(ClientHandler *clientHandler, const std::string &command) {
    MessageHandler::sendGoodbye(clientHandler);
    std::cout << "Client " << clientHandler->getNickname() << " has quit." << command << std::endl;
    m_server.handleClientDisconnect(clientHandler->getSocket());
}

void CommandHandler::handleCap(ClientHandler *clientHandler, const std::string &command) {
    if (command.find("LS") != std::string::npos) {
        MessageHandler::sendCAP(clientHandler);
    }
}
