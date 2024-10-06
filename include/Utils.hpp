#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <cctype>
#include <string.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <vector>

// Utils.cpp
std::string trim(const std::string &str);
void splitCommand(const std::string &command, std::vector<std::string> &parts);

#endif
