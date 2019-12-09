//
// Created by admin2 on 05.12.2019.
//

#ifndef HW7_FORMAT_H
#define HW7_FORMAT_H

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <array>


template <class T>
std::string toString(T&& value) {
    std::stringstream strStream;
    strStream << value;
    return strStream.str();
}

template <std::size_t count>
std::array<std::string, count> ArgsToString() {
    return std::array<std::string, count>();
}

template <std::size_t count,  class T, class... ArgsT>
std::array<std::string, count> ArgsToString(T argValue, ArgsT&&... args) {
    std::array<std::string, count> argStrings = ArgsToString<count, ArgsT...>(std::forward<ArgsT>(args)...);
    argStrings[count - 1 - sizeof...(args)] = toString(argValue);
    return argStrings;
}

template <class... ArgsT>
std::string Format(std::string formatStr, ArgsT&&... args) {
    std::array<std::string, sizeof...(args)> stringArgs = ArgsToString<sizeof...(args), ArgsT...>(std::forward<ArgsT>(args)...);
    std::string result;

    bool isLeftBraceFound = false;
    std::string argIndexStr;
    for (std::size_t i = 0; i < formatStr.size(); ++i) {
        if (formatStr[i] == '{') {
            if (!isLeftBraceFound) {
                isLeftBraceFound = true;
            } else {
                if (!argIndexStr.empty())
                    throw std::runtime_error("Closing brace '}' was not found at position "
                                             + std::to_string(i));
                else
                    throw std::runtime_error("Argument index and closing brace '}' was not found at position "
                                             + std::to_string(i));
            }
        } else if (formatStr[i] == '}') {
            if (isLeftBraceFound && !argIndexStr.empty()) {
                std::size_t argIndex = std::stoull(argIndexStr);
                if (argIndex >= stringArgs.size())
                    throw std::runtime_error("Invalid argument index " + argIndexStr);
                result += stringArgs[argIndex];
                isLeftBraceFound = false;
                argIndexStr = "";
            } else {
                if (!isLeftBraceFound)
                    throw std::runtime_error("Opening brace '{' was not found at position "
                                             + std::to_string(i));
                else
                    throw std::runtime_error("Argument index was not found at position "
                                             + std::to_string(i));
            }
        } else if (isdigit(formatStr[i]) && isLeftBraceFound) {
                argIndexStr += formatStr[i];
        } else if (isLeftBraceFound) {
            throw std::runtime_error(std::string("Invalid symbol '") + formatStr[i]
                                     + "' was found at position " + std::to_string(i));
        } else {
            result += formatStr[i];
        }
    }

    if (isLeftBraceFound)
        if (!argIndexStr.empty())
            throw std::runtime_error("Closing brace '}' was not found at position "
                                     + std::to_string(formatStr.size()));
        else
            throw std::runtime_error("Argument index and closing brace '}' was not found at position "
                                     + std::to_string(formatStr.size()));

    return result;
}


#endif //HW7_FORMAT_H
