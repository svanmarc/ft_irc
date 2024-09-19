#include "server.hpp"

int error_setup_socket_serv(const char *msg)
{
	std::cerr << msg << ": " << std::strerror(errno) << std::endl;
	return -1;
}