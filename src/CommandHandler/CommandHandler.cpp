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
            else if (cmd == "USER") {
                handleUser(command, clientSocket, clientHandler);
            }
            else if (cmd == "USERHOST") {
                std::cout << "USERHOST command received" << std::endl;
                handleUser(command, clientSocket, clientHandler);
            }
            else if (cmd == "MODE") {
                handleMode(command, clientSocket, clientHandler);
            }
            else if (cmd == "WHOIS") {
                handleWhois(clientSocket, clientHandler);
            }
            else if (cmd == "JOIN") {
                std::cout << "JOIN command received" << std::endl;

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


    void CommandHandler::handleCap(const std::string& command, int clientSocket) {
        if (command.find("LS") != std::string::npos) {
            sendResponse(clientSocket, RPL_CAPLS, 
                        "CAP * LS :multi-prefix extended-join account-notify");
            
            sendResponse(clientSocket, RPL_CAPLS, 
                        "Ready to accept commands");
        }
    }

    
    void CommandHandler::handleMode(const std::string& command, int clientSocket, ClientHandler* clientHandler) {
        std::string mode;
        size_t pos = command.find("MODE");
        if (pos != std::string::npos && pos + 5 < command.length()) {
            mode = trim(command.substr(pos + 5));
        }
        
        clientHandler->user.setUserMode(mode);
        
        std::string response = clientHandler->user.getNickname();
        response += " +";
        response += mode;
        sendResponse(clientSocket, RPL_CHANNELMODEIS, response);
    }
    





