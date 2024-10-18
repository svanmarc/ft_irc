#include "Utils.hpp"

std::string toUpper(const std::string &str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

std::string trim(const std::string &str) {
    if (str.empty()) {
        return str;
    }

    std::string::size_type start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return ""; // Tous les caractères sont des espaces
    }

    std::string::size_type end = str.find_last_not_of(" \t\n\r");

    return str.substr(start, end - start + 1);
}

void splitCommand(const std::string &command, std::vector<std::string> &parts) {
    std::string::size_type start = 0, end = 0;

    while (start < command.length() && command[start] == ' ') {
        ++start;
    }

    while (start < command.length()) {
        if (command[start] == ':') {
            parts.push_back(command.substr(start + 1));
            break;
        }

        end = command.find(' ', start);
        if (end == std::string::npos) {
            parts.push_back(command.substr(start));
            break;
        }

        parts.push_back(command.substr(start, end - start));
        start = end + 1;

        // Skip any additional spaces
        while (start < command.length() && command[start] == ' ') {
            ++start;
        }
    }
}
