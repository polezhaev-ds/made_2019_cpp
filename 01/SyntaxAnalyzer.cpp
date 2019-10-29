//
// Created by admin2 on 28.10.2019.
//

#include <string>
#include <sstream>
#include "SyntaxAnalyzer.h"
#include "AnalyzerException.h"

int SyntaxAnalyzer::analyzeAndCalculate() {
    currentIndex = 0;
    int result = expressionRecursion();
    if (currentIndex < tokens.size()) {
        int position = tokens[currentIndex]->getPosition();
        std::string message;
        if (tokens[currentIndex]->getType()==TokenType::RightBracket)
            message = "Error! Left brace is not found for right brace!";
        else
            message = "Error! Invalid token!";
        throw AnalyzerException(message, position);
    }
    return result;
}

int SyntaxAnalyzer::expressionRecursion() {
    int result = termRecursion();
    if (currentIndex < tokens.size()) {
        while (currentIndex < tokens.size() && (tokens[currentIndex]->getType() == TokenType::OperationPlus ||
                tokens[currentIndex]->getType() == TokenType::OperationMinus)) {
            if (tokens[currentIndex]->getType() == TokenType::OperationPlus) {
                currentIndex++;
                result += termRecursion();
            } else if (tokens[currentIndex]->getType() == TokenType::OperationMinus) {
                currentIndex++;
                result -= termRecursion();
            }
        }
    }
    return result;
}

int SyntaxAnalyzer::termRecursion() {
    int result = monadRecursion();
    if (currentIndex < tokens.size()) {
        while (currentIndex < tokens.size() && (tokens[currentIndex]->getType() == TokenType::OperationMultiply ||
                tokens[currentIndex]->getType() == TokenType::OperationDivide)) {
            if (tokens[currentIndex]->getType() == TokenType::OperationMultiply) {
                currentIndex++;
                result *= monadRecursion();
            } else if (tokens[currentIndex]->getType() == TokenType::OperationDivide) {
                int divisionIndex = currentIndex;
                currentIndex++;
                int monad = monadRecursion();
                if (monad == 0) {
                    throw AnalyzerException("Error! Division by zero!",
                                             tokens[divisionIndex]->getPosition());
                }
                result /= monad;
            }
        }
    }
    return result;
}

int SyntaxAnalyzer::monadRecursion() {
    if (currentIndex < tokens.size() && tokens[currentIndex]->getType() == TokenType::OperationMinus) {
        currentIndex++;
        int atom = atomRecursion();
        return -atom;
    } else {
        return atomRecursion();
    }
}

int SyntaxAnalyzer::atomRecursion() {
    if (currentIndex < tokens.size() && tokens[currentIndex]->getType() == TokenType::LeftBracket) {
        currentIndex++;
        int number = expressionRecursion();
        if (currentIndex < tokens.size() && tokens[currentIndex]->getType() == TokenType::RightBracket) {
            currentIndex++;
            return number;
        } else {
            int position;
            if (currentIndex == tokens.size()) {
                position = expression.length();
            } else {
                position = tokens[currentIndex]->getPosition();
            }
            throw AnalyzerException("Error! Right brace is not found!", position);
        }
    }
    return number();
}

int SyntaxAnalyzer::number() {
    if (currentIndex == tokens.size()) {
        throw AnalyzerException("Error! Incomplete expression!", expression.length());
    }
    if (currentIndex < tokens.size() && tokens[currentIndex]->getType() == TokenType::Number) {
        int number = std::static_pointer_cast<NumberToken>(tokens[currentIndex])->getNumber();
        currentIndex++;
        return number;
    }
    int position = tokens[currentIndex]->getPosition();
    throw AnalyzerException("Error! Invalid token!", position);
}