#include <csignal>
#include <cstdlib>
#include <iostream>
#include "Server.hpp"

// Pointeur global pour l'instance du serveur
Server *serverInstance = NULL;

// Gestionnaire de signaux
void signalHandler(int signal) {
    if (serverInstance == NULL) {
        std::cerr << RED << "Server not initialized. Exiting..." << RESET << std::endl;
        return;
    }

    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << BLUE << "\n🛰️ Received signal (" << signal << ")...." << RESET << std::endl;
        if (serverInstance->isRunning()) {
            serverInstance->stop();
        }
        std::cout << BLUE << "Exiting...\nWelcome to the real world. 🌎" << RESET << std::endl;
        delete serverInstance;
        serverInstance = NULL;
        exit(0);
    } else {
        std::cerr << RED << "Received unknown signal (" << signal << "). Ignoring..." << RESET << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << RED << "Usage: ./ircserv <port> <password>" << RESET << std::endl;
        return 1;
    }

    const int port = std::atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        std::cerr << RED << "Invalid port number. Must be between 1 and 65535." << RESET << std::endl;
        return 1;
    }

    const std::string serverPassword = argv[2];

    try {
        // Créer l'instance locale du serveur
        serverInstance = new Server(port, serverPassword);
        // Configurer les signaux après la création de l'instance du serveur
        std::signal(SIGINT, signalHandler); // Intercepter Ctrl+C
        std::signal(SIGTERM, signalHandler); // Intercepter `kill`
        // Démarrer le serveur
        serverInstance->start();
    } catch (const std::exception &e) {
        std::cerr << RED << "Server error: " << e.what() << RESET << std::endl;
        delete serverInstance;
        return 1;
    }

    return 0;
}
