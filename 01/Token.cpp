//
// Created by admin2 on 28.10.2019.
//

#include <regex>
#include <sstream>
#include "Token.h"
#include "NumberToken.h"
#include "Exception.h"

TokenType Token::getType() const {
    return type;
}

int Token::getPosition() const {
    return position;
}

std::string Token::toString() const {
    switch (type) {
        case TokenType::OperationPlus:
            return std::string("+");
        case TokenType::OperationMinus:
            return std::string("-");
        case TokenType::OperationMultiply:
            return std::string("*");
        case TokenType::OperationDivide:
            return std::string("/");
        case TokenType::LeftBracket:
            return std::string("(");
        case TokenType::RightBracket:
            return std::string(")");
    }
    return std::string("");
}

int Token::getLength() const {
    return 1;
}

#ifndef NDEBUG
std::string Token::toDebugString() const {
    std::stringstream ss;
    ss << "(\"" << toString() << "\", " << position << ")";
    return ss.str();
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