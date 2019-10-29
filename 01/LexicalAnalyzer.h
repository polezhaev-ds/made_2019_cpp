//
// Created by admin2 on 28.10.2019.
//

#ifndef HW1_LEXICALANALYZER_H
#define HW1_LEXICALANALYZER_H

#include <vector>
#include <string>
#include <memory>
#include "Token.h"

class LexicalAnalyzer {
public:
    static std::vector<std::shared_ptr<Token>> Analyze(const std::string& expression);
private:
    static bool IsDigitSymbol(char ch);
    static bool IsOperationSymbol(char ch);
    static bool IsSpaceSymbol(char ch);
};


#endif //HW1_LEXICALANALYZER_H
