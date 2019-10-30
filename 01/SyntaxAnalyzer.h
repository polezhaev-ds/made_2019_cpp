//
// Created by admin2 on 28.10.2019.
//

#ifndef HW1_SYNTAXANALYZER_H
#define HW1_SYNTAXANALYZER_H

#include <utility>
#include <vector>
#include <string>
#include "Token.h"
#include "NumberToken.h"

class SyntaxAnalyzer {
public:
    explicit SyntaxAnalyzer(const std::vector<std::shared_ptr<Token>>& tokens, const std::string& expression):
            tokens(tokens),
            expression(expression),
            currentIndex(0)
    {
    }

    int analyzeAndCalculate();
private:
    const std::vector<std::shared_ptr<Token>>& tokens;
    const std::string& expression;
    int currentIndex;

    int expressionRecursion();
    int termRecursion();
    int monadRecursion();
    int atomRecursion();
    int number();
};


#endif //HW1_SYNTAXANALYZER_H
