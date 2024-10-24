// #include "CommandHandler.hpp"

// bool CommandHandler::validateCommandFormat(ClientHandler *clientHandler, const std::vector<std::string> &parts) {
//     if (parts.size() < 2) {
//         MessageHandler::sendErrorNoTarget(clientHandler);
//         return false;
//     }
//     if (parts.size() > 3) {
//         MessageHandler::sendErrortooManyTargets(clientHandler);
//         return false;
//     }
//     return true;
// }

// std::string CommandHandler::getChannelNameFromCommand(ClientHandler *clientHandler,
//                                                       const std::vector<std::string> &parts) {
//     if (parts.size() == 3) { // A channel was specified
//         if (parts[2][0] != '#') {
//             MessageHandler::sendErrorNoSuchChannel(clientHandler, parts[2]);
//             return "";
//         }
//         return trim(parts[2]);
//     } else { // No channel specified, use the last joined channel
//         if (clientHandler->getChannels().empty()) {
//             MessageHandler::sendErrorNotInChannel(clientHandler, "current");
//             return "";
//         }
//         return clientHandler->getChannels().back();
//     }
// }

// bool CommandHandler::validateChannel(ClientHandler *clientHandler, const std::string &channelName) {
//     if (!clientHandler->getServer()->checkIfChannelExists(channelName)) {
//         MessageHandler::sendErrorNoSuchChannel(clientHandler, channelName);
//         return false;
//     }

//     Channel &channel = clientHandler->getServer()->getChannel(channelName);
//     if (!channel.checkIfClientIsInChannel(clientHandler)) {
//         MessageHandler::sendErrorNotInChannel(clientHandler, channelName);
//         return false;
//     }
//     return true;
// }
