//
// Created by admin2 on 28.10.2019.
//

#include "AnalyzerException.h"



int AnalyzerException::getPostion() const {
    return position;
}

std::string AnalyzerException::getMessage() const {
    return Exception::getMessage() + " Position " + std::to_string(position) + ".";
}

