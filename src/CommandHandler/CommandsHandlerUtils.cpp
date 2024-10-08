//
// Created by martin on 08/10/2024.
//
#include "CommandHandler.hpp"

void CommandHandler::handleWhois(int clientSocket, ClientHandler* clientHandler) {
    std::string response = "NICK ";
    response += clientHandler->user.getNickname();
    sendResponse(clientSocket, RPL_WELCOME, response);
}

void CommandHandler::sendResponse(int clientSocket, int code, const std::string& message) {
    std::string response = ":localhost ";
    std::ostringstream oss;
    oss << code;
    response += oss.str();
    response += " " + message + "\r\n";
    std::cout << "Sending response: " << response << std::endl;
    send(clientSocket, response.c_str(), response.length(), 0);
}

void CommandHandler::sendResponse(int clientSocket, const std::string& message) {
    std::string response = ":localhost ";
    std::ostringstream oss;
    response += oss.str();
    response += " " + message + "\r\n";
    send(clientSocket, response.c_str(), response.length(), 0);
}

std::string CommandHandler::parseCommand(const std::string& fullCommand) {
    std::istringstream iss(fullCommand);
    std::string cmd;
    iss >> cmd;
    return cmd;
}