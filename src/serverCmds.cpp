#include "server.hpp"

void nick_command(const std::string &command, int clientSocket)
{
	std::string nick = command.substr(5);
	std::cout << "Nick: " << nick << std::endl;
	const char *nickResponse = ":localhost 001 Welcome to the Internet Relay Network user!user@localhost\r\n";
	send(clientSocket, nickResponse, strlen(nickResponse), 0);
}

// exemple de nick_command

// void nick_command(const std::string &command, int clientSocket)
// {
//     // Vérification que la commande est bien au format "NICK <pseudo>"
//     if (command.length() <= 5 || command[4] != ' ')
//     {
//         const char *errorResponse = ":localhost 431 ERR_NONICKNAMEGIVEN :No nickname given\r\n";
//         send(clientSocket, errorResponse, strlen(errorResponse), 0);
//         std::cerr << "Error: No nickname given." << std::endl;
//         return;
//     }

//     // Extraction du pseudo après "NICK "
//     std::string nick = command.substr(5);
//     // Supprimer les espaces en début et fin de pseudo si nécessaire
//     nick.erase(0, nick.find_first_not_of(' '));
//     nick.erase(nick.find_last_not_of(' ') + 1);

//     // Vérifier si le pseudo est vide après nettoyage
//     if (nick.empty())
//     {
//         const char *errorResponse = ":localhost 431 ERR_NONICKNAMEGIVEN :No nickname given\r\n";
//         send(clientSocket, errorResponse, strlen(errorResponse), 0);
//         std::cerr << "Error: No nickname given after processing." << std::endl;
//         return;
//     }

//     // Validation simple du pseudo (ajoutez vos propres règles ici)
//     if (nick.length() > 9) // Limite typique de longueur pour un pseudo IRC
//     {
//         const char *errorResponse = ":localhost 432 ERR_ERRONEUSNICKNAME :Erroneous nickname\r\n";
//         send(clientSocket, errorResponse, strlen(errorResponse), 0);
//         std::cerr << "Error: Erroneous nickname." << std::endl;
//         return;
//     }

//     // Si tout est correct, envoyer une réponse de bienvenue avec le pseudo
//     std::string nickResponse = ":localhost 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!user@localhost\r\n";
//     send(clientSocket, nickResponse.c_str(), nickResponse.length(), 0);
//     std::cout << "Handled NICK command for: " << nick << std::endl;
// }
