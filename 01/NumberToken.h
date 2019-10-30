//
// Created by admin2 on 28.10.2019.
//

#ifndef HW1_NUMBERTOKEN_H
#define HW1_NUMBERTOKEN_H

#include "Token.h"
#include <string>

class NumberToken: public Token {
public:
    NumberToken(std::string numberStr, int position):
        Token(TokenType::Number, position),
        numberStr(std::move(numberStr))
    {
    }

    std::string getValue() const override;

private:
    std::string numberStr;

};


#endif //HW1_NUMBERTOKEN_H
