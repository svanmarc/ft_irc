#include "CommandHandler.hpp"

void CommandHandler::handleJoinChannel(const std::string &command, ClientHandler *clientHandler) {
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 2) {
        MessageHandler::sendErrorJoinChannel(clientHandler, "");
        return;
    }
    std::string const channelName = trim(parts[1]);
    if (channelName[0] != '#') {
        MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
        return;
    }
    const bool joinStatus = getServer().joinChannel(clientHandler, channelName);
    std::cout << "Join status: " << joinStatus << std::endl;
    if (joinStatus) {
        Channel const newChannel = getServer().getChannel(channelName);
        MessageHandler::sendWelcomeToChannel(clientHandler, newChannel);
        //MessageHandler::sendNewMemberToChannel(clientHandler, newChannel);

    } else {
        MessageHandler::sendErrorJoinChannel(clientHandler, channelName);
    }

}