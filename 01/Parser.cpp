//
// Created by admin2 on 28.10.2019.
//

#include <iostream>
#include <vector>
#include <memory>
#include "Parser.h"
#include "AnalyzerException.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"

int Parser::ParseAndCompute(const std::string& expression) {
    std::vector<std::shared_ptr<Token>> tokens = LexicalAnalyzer::Analyze(expression);

#ifndef NDEBUG
    std::cout << "Tokens:" << std::endl;
    for (auto & token : tokens)
        std::cout << token->toDebugString() << " ";
    std::cout << std::endl;
#endif

    if (tokens.empty()) {
        throw AnalyzerException("Error! Expression is empty or contains only spaced or tabs!", 0);
    }

    SyntaxAnalyzer analyzer = SyntaxAnalyzer(tokens, expression);
    return analyzer.analyzeAndCalculate();
}
