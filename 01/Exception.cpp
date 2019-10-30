//
// Created by admin2 on 28.10.2019.
//

#include "Exception.h"


const std::string& Exception::getMessage() const {
    return message;
}

const char* Exception::what() const noexcept {
    return message.c_str();
}