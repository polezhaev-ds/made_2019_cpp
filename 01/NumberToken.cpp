//
// Created by admin2 on 28.10.2019.
//

#include "NumberToken.h"

int NumberToken::getNumber() const {
    return number;
}

std::string NumberToken::toString() const {
    return std::to_string(number);
}

int NumberToken::getLength() const {
    return length;
}