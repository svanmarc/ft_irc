#include "CommandHandler.hpp"

const std::string CommandHandler::WELCOME_MESSAGE = "Welcome to the Internet Relay Network 2024 ";

void CommandHandler::handleCommand(const std::string& command, int clientSocket, ClientHandler *clientHandler)
{
        try {
            std::cout << "Received command: " << command << std::endl;
            
            std::string cmd = parseCommand(command);
            
            // Convert command to uppercase for case-insensitive comparison
            std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);
            
            if (cmd == "CAP") {
                handleCap(command, clientSocket);
            }
            else if (cmd == "NICK") {
                handleNick(command, clientSocket, clientHandler);
            }
            else if (cmd == "MODE") {
                handleMode(command, clientSocket, clientHandler);
            }
            else if (cmd == "WHOIS") {
                handleWhois(clientSocket, clientHandler);
            }
            else {
                sendResponse(clientSocket, ERR_UNKNOWNCOMMAND, 
                            std::string("Unknown command: ") + cmd);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error handling command: " << e.what() << std::endl;
            sendResponse(clientSocket, ERR_UNKNOWNCOMMAND, 
                        "Error processing command");
        }
}

    
    std::string CommandHandler::parseCommand(const std::string& fullCommand) {
        std::istringstream iss(fullCommand);
        std::string cmd;
        iss >> cmd;
        return cmd;
    }
    
    void CommandHandler::handleCap(const std::string& command, int clientSocket) {
        if (command.find("LS") != std::string::npos) {
            sendResponse(clientSocket, RPL_CAPLS, 
                        "CAP * LS :multi-prefix extended-join account-notify");
            
            sendResponse(clientSocket, RPL_CAPLS, 
                        "Ready to accept commands");
        }
    }
    
    void CommandHandler::handleNick(const std::string& command, int clientSocket, ClientHandler* clientHandler) {
        std::string nickname;
        size_t pos = command.find("NICK");
        if (pos != std::string::npos && pos + 5 < command.length()) {
            nickname = trim(command.substr(pos + 5));
        }
        
        if (nickname.empty()) {
            sendResponse(clientSocket, ERR_UNKNOWNCOMMAND, 
                        "No nickname given");
            return;
        }
        
        clientHandler->m_user.setNickname(nickname);
        
        std::string welcomeMessage = "Welcome to the Internet Relay Network 2024 ";
        welcomeMessage += nickname;
        sendResponse(clientSocket, RPL_WELCOME, welcomeMessage);
    }
    
    void CommandHandler::handleMode(const std::string& command, int clientSocket, ClientHandler* clientHandler) {
        std::string mode;
        size_t pos = command.find("MODE");
        if (pos != std::string::npos && pos + 5 < command.length()) {
            mode = trim(command.substr(pos + 5));
        }
        
        clientHandler->m_user.setUserMode(mode);
        
        std::string response = clientHandler->m_user.getNickname();
        response += " +";
        response += mode;
        sendResponse(clientSocket, RPL_CHANNELMODEIS, response);
    }
    
    void CommandHandler::handleWhois(int clientSocket, ClientHandler* clientHandler) {
        std::string response = "NICK ";
        response += clientHandler->m_user.getNickname();
        sendResponse(clientSocket, RPL_WELCOME, response);
    }
    
      void CommandHandler::sendResponse(int clientSocket, int code, const std::string& message) {
        std::string response = ":localhost ";
        std::ostringstream oss;
        oss << code;
        response += oss.str();
        response += " " + message + "\r\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    }