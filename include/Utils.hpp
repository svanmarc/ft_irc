#ifndef UTILS_HPP
#define UTILS_HPP

#include "Irc.hpp"

// Utils.cpp
std::string trim(const std::string &str);
void splitCommand(const std::string &command, std::vector<std::string> &parts);
std::string toUpper(const std::string &str);

#endif
