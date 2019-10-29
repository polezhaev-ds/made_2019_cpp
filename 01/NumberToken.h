//
// Created by admin2 on 28.10.2019.
//

#ifndef HW1_NUMBERTOKEN_H
#define HW1_NUMBERTOKEN_H

#include "Token.h"
#include <string>

class NumberToken: public Token {
public:
    NumberToken(const std::string& numberStr, int position):
        Token(TokenType::Number, position),
        number(std::stoi(numberStr)),
        length(numberStr.length())
    {

    }

    int getNumber() const;
    std::string toString() const override;
    int getLength() const override;

private:
    int number;
    int length;

};


#endif //HW1_NUMBERTOKEN_H
