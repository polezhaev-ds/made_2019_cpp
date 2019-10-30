//
// Created by admin2 on 28.10.2019.
//

#ifndef HW1_LEXICALANALYZER_H
#define HW1_LEXICALANALYZER_H

#include <vector>
#include <string>
#include <memory>
#include "Token.h"

std::vector<std::shared_ptr<Token>> LexicalAnalysis(const std::string& expression);


#endif //HW1_LEXICALANALYZER_H
