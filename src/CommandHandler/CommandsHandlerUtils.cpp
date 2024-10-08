#include "CommandHandler.hpp"

void CommandHandler::sendResponse(int clientSocket, int code, const std::string &message) {
    std::string response = ":localhost ";
    if (code != 0) // N'ajouter le code que s'il est différent de 0
    {
        std::ostringstream oss;
        oss << code;
        response += oss.str() + " ";
    }
    response += message + "\r\n";
    std::cout << "Sending response: " << response << std::endl;
    send(clientSocket, response.c_str(), response.length(), 0);
}

void CommandHandler::sendResponse(int clientSocket, const std::string &message) {
    sendResponse(clientSocket, 0, message);
}

std::string CommandHandler::parseCommand(const std::string& fullCommand) {
    std::istringstream iss(fullCommand);
    std::string cmd;
    iss >> cmd;
    return cmd;
}