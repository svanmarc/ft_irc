#pragma once
#include <string>
#include <cctype>
#include <string.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>


std::string trim(const std::string& str);
void splitCommand(const std::string& command, std::vector<std::string> &parts);