
#include "CommandHandler.hpp"
#include "MessageHandler.hpp"

bool CommandHandler::checkNickname(const std::string &nickname, ClientHandler *clientHandler) {
    if (nickname.empty()) {
        MessageHandler::sendErrorNoNickNameGiven(clientHandler);
        return false;
    }
    if (nickname.size() > 9) {
        MessageHandler::sendErrorNickNameTooLong(clientHandler);
        return false;
    }
    if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") != std::string::npos) {
        MessageHandler::sendErrorNickNameInvalidCharacters(clientHandler);
        return false;
    }
    return true;
}

void CommandHandler::handleNick(const std::string &command, ClientHandler *clientHandler) {
    // manque la verification si il existe deja !!!!!
    std::vector<std::string> parts;
    splitCommand(command, parts);
    if (parts.size() < 2) {
        MessageHandler::sendErrorNoNickNameGiven(clientHandler);
        return;
    }
    if (parts.size() > 2) {
        MessageHandler::sendErrorNickNameInvalidCharacters(clientHandler);
        return;
    }
    std::string const nickname = trim(parts[1]);
    if (!checkNickname(nickname, clientHandler)) {
        return;
    }
    std::string const oldNickname = clientHandler->getUser().getNickname();
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
