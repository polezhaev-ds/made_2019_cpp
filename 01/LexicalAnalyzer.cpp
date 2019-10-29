//
// Created by admin2 on 28.10.2019.
//

#include "LexicalAnalyzer.h"
#include "AnalyzerException.h"
#include <sstream>

std::vector<std::shared_ptr<Token>> LexicalAnalyzer::Analyze(const std::string& expression)  {
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
            std::stringstream messageStream;
            messageStream << "Error! Unknown symbol '" << expression[i] << "'!";
            throw AnalyzerException(messageStream.str(), i);
        } else {
            i++;
        }
    }
    return tokens;
}

bool LexicalAnalyzer::IsDigitSymbol(char ch)  {
    return ch >= '0' && ch <= '9';
}

bool LexicalAnalyzer::IsOperationSymbol(char ch)  {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')';
}

bool LexicalAnalyzer::IsSpaceSymbol(char ch)  {
    return ch == ' ' || ch == '\t';
}


