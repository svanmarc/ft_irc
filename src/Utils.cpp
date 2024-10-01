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