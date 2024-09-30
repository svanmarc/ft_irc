#include "server.hpp"
#include <csignal>
#include <cstdlib>

Server *serverInstance = NULL;

void handleSignal(int signal)
{
	if(signal == SIGINT)
	{
		serverInstance->stop();
		exit(0);
	}
}


int main()
{
	Server server(12345);
	serverInstance = &server;
	
	std::signal(SIGINT, handleSignal);
	server.start();
	return 0;
}