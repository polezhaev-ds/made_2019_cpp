//
// Created by admin2 on 28.10.2019.
//

#include "LexicalAnalyzer.h"
#include "AnalyzerException.h"


static bool IsDigitSymbol(char ch)  {
    return ch >= '0' && ch <= '9';
}

static bool IsOperationSymbol(char ch)  {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')';
}

static bool IsSpaceSymbol(char ch)  {
    return ch == ' ' || ch == '\t';
}

std::vector<std::shared_ptr<Token>> LexicalAnalysis(const std::string& expression)  {
    std::vector<std::shared_ptr<Token>> tokens;
    int i = 0;
    while (i < expression.length()) {
        if (IsOperationSymbol(expression[i])) {
            tokens.push_back(Token::CreateToken(expression.substr(i, 1), i));
            i++;
        } else if (IsDigitSymbol(expression[i])) {
            int startIndex = i;
            int digitsCount = 0;
            while (i < expression.length() && IsDigitSymbol(expression[i])) {
                digitsCount++;
                i++;
            }
            tokens.push_back(Token::CreateToken(expression.substr(startIndex, digitsCount), startIndex));
        } else if (!IsSpaceSymbol(expression[i])) {
            std::string message = std::string("Error! Unknown symbol '") + expression[i] + "'!";
            throw AnalyzerException(message, i);
        } else {
            i++;
        }
    }
    return tokens;
}



