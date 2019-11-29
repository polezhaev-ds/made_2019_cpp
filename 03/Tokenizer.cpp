//
// Created by admin2 on 06.11.2019.
//

#include "Tokenizer.h"

enum TokenType { Unknown, Number, String };

void Tokenizer::parse(const std::string& str) const {
    int tokenStartIndex = -1;
    std::size_t tokenWLength = 0;
    TokenType tokenType = TokenType::Unknown;

    if (onParseStarted != nullptr)
        onParseStarted(str);

    for (std::size_t i = 0; i < str.size() + 1; i++)
        if (i != str.size() && std::isdigit(str[i])) {

            if (tokenType == TokenType::Unknown)
                tokenType = TokenType::Number;

            if (tokenWLength == 0) {
                tokenStartIndex = i;
            }
            tokenWLength++;

        } else if (i == str.size() || std::isspace(str[i])) {
            if (tokenWLength != 0) {

                std::string nextToken = str.substr(tokenStartIndex, tokenWLength);

                if (onNextToken != nullptr)
                    onNextToken(nextToken, tokenStartIndex);
                if (tokenType == TokenType::Number && onNextNumericToken != nullptr)
                    onNextNumericToken(nextToken, tokenStartIndex);
                else if (tokenType == TokenType::String && onNextStringToken != nullptr)
                    onNextStringToken(nextToken, tokenStartIndex);

                tokenWLength = 0;
                tokenType = TokenType::Unknown;
            }
        } else {

            if (tokenType == TokenType::Unknown || tokenType == TokenType::Number)
                tokenType = TokenType::String;

            if (tokenWLength == 0) {
                tokenStartIndex = i;
            }
            tokenWLength++;

        }

    if (onParseFinished != nullptr)
        onParseFinished(str);
}

Tokenizer& Tokenizer::setOnParseStartedHandler(const ParsingHandler& handler) {
    onParseStarted = handler;
    return *this;
}

Tokenizer& Tokenizer::setOnParseFinishedHandler(const ParsingHandler& handler)  {
    onParseFinished = handler;
    return *this;
}

Tokenizer& Tokenizer::setOnNextTokenHandler(const NextTokenParsedHandler& handler) {
    onNextToken = handler;
    return *this;
}

Tokenizer& Tokenizer::setOnNextNumericTokenHandler(const NextTokenParsedHandler& handler) {
    onNextNumericToken = handler;
    return *this;
}

Tokenizer& Tokenizer::setOnNextStringTokenHandler(const NextTokenParsedHandler& handler) {
    onNextStringToken = handler;
    return *this;
}

Tokenizer& Tokenizer::unsetOnParseStartedHandler() {
    onParseStarted = nullptr;
    return *this;
}

Tokenizer& Tokenizer::unsetOnParseFinishedHandler() {
    onParseFinished = nullptr;
    return *this;
}

Tokenizer& Tokenizer::unsetOnNextTokenHandler() {
    onNextToken = nullptr;
    return *this;
}

Tokenizer& Tokenizer::unsetOnNextNumericTokenHandler() {
    onNextNumericToken = nullptr;
    return *this;
}

Tokenizer& Tokenizer::unsetOnNextStringTokenHandler() {
    onNextStringToken = nullptr;
    return *this;
}
