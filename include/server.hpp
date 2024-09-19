#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <cerrno>

// server.cpp
void start_server(int port);
int setup_server_socket(int port);
void handle_client(int clientSocket);
void handle_command(const std::string &command, int clientSocket);

// serverTools.cpp
int error_setup_socket_serv(const char *msg);

// serverCmds.cpp
void nick_command(const std::string &command, int clientSocket);

#endif