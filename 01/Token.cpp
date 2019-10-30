//
// Created by admin2 on 28.10.2019.
//

#include <regex>
#include "Token.h"
#include "NumberToken.h"
#include "Exception.h"

TokenType Token::getType() const {
    return type;
}

int Token::getPosition() const {
    return position;
}

std::string Token::getValue() const {
    switch (type) {
        case TokenType::OperationPlus:
            return "+";
        case TokenType::OperationMinus:
            return "-";
        case TokenType::OperationMultiply:
            return "*";
        case TokenType::OperationDivide:
            return "/";
        case TokenType::LeftBracket:
            return "(";
        case TokenType::RightBracket:
            return ")";
    }
    return "";
}

std::string Token::toString() const {
    return getValue();
}

#ifndef NDEBUG
std::string Token::toDebugString() const {
    return "(\"" + toString() + "\", " + std::to_string(position) + ")";
}
#endif

std::shared_ptr<Token> Token::CreateToken(const std::string& tokenStr, int position)  {
    if (tokenStr == "+")
        return std::make_shared<Token>(TokenType::OperationPlus, position);
    if (tokenStr == "-")
        return std::make_shared<Token>(TokenType::OperationMinus, position);
    if (tokenStr == "*")
        return std::make_shared<Token>(TokenType::OperationMultiply, position);
    if (tokenStr == "/")
        return std::make_shared<Token>(TokenType::OperationDivide, position);
    if (tokenStr == "(")
        return std::make_shared<Token>(TokenType::LeftBracket, position);
    if (tokenStr == ")")
        return std::make_shared<Token>(TokenType::RightBracket, position);
    if (std::regex_match(tokenStr, std::regex("\\d+")))
        return std::make_shared<NumberToken>(tokenStr, position);
    throw Exception("Unknown token \"" + tokenStr + "\"!");
}