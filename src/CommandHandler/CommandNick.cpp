
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
    if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") !=
        std::string::npos) {
        MessageHandler::sendErrorNickNameInvalidCharacters(clientHandler);
        return false;
    }
    return true;
}

// void CommandHandler::handleNick(ClientHandler *clientHandler, const std::string &command) {
//     // manque la verification si il existe deja !!!!!
//     std::vector<std::string> parts;
//     splitCommand(command, parts);
//     if (parts.size() < 2 || parts[1].empty()) {
//         MessageHandler::sendErrorNoNickNameGiven(clientHandler);
//         return;
//     }
//     std::string nickname = parts[1];
//     std::string oldNickname = clientHandler->getUser().getNickname();
//     if (oldNickname == nickname) {
//         return;
//     }
//     bool const nicknameExiste=clientHandler->getServer()->checkNickname(nickname);
//     if (nicknameExiste) {
//         MessageHandler::sendErrorNickNameAlreadyExists(clientHandler, nickname);
//         return;
//     }
//     if (oldNickname.empty()) {
//         // Envoyer un message de changement de nickname
//         std::cout << "New nickname: " << nickname << "Register" << std::endl;
//     } else {
//         // Envoyer un message de bienvenue
//         MessageHandler::sendChangeNickName(clientHandler, oldNickname, nickname);
//     }
//     clientHandler->getUser().setNickname(nickname);
// }

void CommandHandler::handleNick(ClientHandler *clientHandler, const std::string &command) {
    std::vector<std::string> parts;
    splitCommand(command, parts);

    if (parts.size() < 2 || parts[1].empty()) {
        MessageHandler::sendErrorNoNickNameGiven(clientHandler);
        return;
    }

    std::string newNickname = parts[1];
    std::string oldNickname = clientHandler->getUser().getNickname();

    // Vérifier la validité du nouveau pseudo
    if (!checkNickname(newNickname, clientHandler)) {
        return;
    }

    // Si le pseudo est le même que l'ancien, ne rien faire
    if (oldNickname == newNickname) {
        return;
    }

    // Vérifier si le nouveau pseudo existe déjà
    if (clientHandler->getServer()->checkNickname(newNickname)) {
        MessageHandler::sendErrorNickNameAlreadyExists(clientHandler, newNickname);
        return;
    }

    // Si un ancien pseudo est défini, le libérer avant de changer
    if (!oldNickname.empty()) {
        std::cout << "Releasing old nickname: " << oldNickname << std::endl;
        // Logique pour libérer le pseudo si nécessaire (peut-être supprimer l'ancien pseudo de la liste des
        // utilisateurs)
    }

    // Mettre à jour le pseudo du client
    clientHandler->getUser().setNickname(newNickname);

    // Si l'utilisateur a changé de pseudo, notifier les autres utilisateurs
    if (!oldNickname.empty()) {
        MessageHandler::sendChangeNickName(clientHandler, oldNickname, newNickname);
        std::cout << "Nickname changed from " << oldNickname << " to " << newNickname << std::endl;
    } else {
        std::cout << "New nickname: " << newNickname << " registered." << std::endl;
    }
}
