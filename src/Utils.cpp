#include "Utils.hpp"


std::string trim(const std::string& str) {
    if (str.empty()) {
        return str;
    }

    // Variables to keep track of the first and last non-whitespace characters
    std::string::size_type start = 0;
    std::string::size_type end = str.length() - 1;

    // Find the first non-whitespace character
    while (start <= end && isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }

    // All characters are whitespace
    if (start > end) {
        return "";
    }

    // Find the last non-whitespace character
    while (end > start && isspace(static_cast<unsigned char>(str[end]))) {
        --end;
    }

    // Return the trimmed substring
    return str.substr(start, end - start + 1);
}

void splitCommand(const std::string& command, std::vector<std::string> &parts){
    std::string::size_type start = 0;
    std::string::size_type end = 0;

    while (start < command.length() && command[start] == ' '){
        start++;
    }
    while (start < command.length()) {
        if (command[start] == ':'){
            parts.push_back(command.substr(start + 1));
            break;
        }

        end = command.find(' ', start);
        if (end == std::string::npos){
            parts.push_back(command.substr(start));
            break;
        }
        parts.push_back(command.substr(start, end - start));
        start = end + 1;
        while (start < command.length() && command[start] == ' '){
            start++;
        }
    }
};