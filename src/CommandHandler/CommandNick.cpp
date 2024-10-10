
#include "CommandHandler.hpp"
#include "MessageHandler.hpp"

void CommandHandler::handleNick(const std::string &command, ClientHandler *clientHandler) {
    // manque la verification si il existe deja !!!!!
    std::string nickname;
    size_t pos = command.find("NICK");
    if (pos != std::string::npos && pos + 5 < command.length()) {
        nickname = trim(command.substr(pos + 5));
    }
    if (nickname.empty()) {
        MessageHandler::sendErrorNoNickNameGiven(clientHandler);
        return;
    }
    std::string oldNickname = clientHandler->getUser().getNickname();
    if (oldNickname == nickname) {
        return;
    }
    bool const nicknameExiste=clientHandler->getServer()->checkNickname(nickname);
    if (nicknameExiste) {
        MessageHandler::sendErrorNickNameAlreadyExists(clientHandler, nickname);
        return;
    }
    if (oldNickname.empty()) {
        // Envoyer un message de changement de nickname
        std::cout << "New nickname: " << nickname << "Register" << std::endl;
    } else {
        // Envoyer un message de bienvenue
        MessageHandler::sendChangeNickName(clientHandler, oldNickname, nickname);
    }
    clientHandler->getUser().setNickname(nickname);
}
