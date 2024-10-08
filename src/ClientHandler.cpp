#include "ClientHandler.hpp"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "CommandHandler.hpp"

ClientHandler::ClientHandler(int socket, Server *serverRef) : m_clientSocket(socket), m_server(serverRef) {}
ClientHandler::~ClientHandler() { close(m_clientSocket); }

void ClientHandler::handlerClient() {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(m_clientSocket, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            std::cerr << "Client disconnected or error reading" << std::endl;
            break;
        }
        std::string command(buffer);
        std::cout << "Received command: " << command << std::endl;
        readCommand(command);
    }
}

void ClientHandler::readCommand(const std::string &command) {
    CommandHandler commandHandler;
    commandHandler.handleCommand(command, m_clientSocket, this);
}

void ClientHandler::sendResponse(const std::string &response) {
    send(m_clientSocket, response.c_str(), response.length(), 0);
}


bool ClientHandler::joinChannel(const std::string &channel) { return (m_server->joinChannel(this->user, channel)); }

void ClientHandler::leaveChannel(const std::string &channel) {
    for (size_t i = 0; i < channels.size(); i++) {
        if (channels[i] == channel) {
            channels.erase(channels.begin() + i);
            break;
        }
    }
}
