#include "CommandHandler.hpp"

std::string CommandHandler::parseCommand(const std::string &fullCommand) {
    std::istringstream iss(fullCommand);
    std::string cmd;
    iss >> cmd;
    return cmd;
}
