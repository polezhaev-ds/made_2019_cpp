//
// Created by admin2 on 28.10.2019.
//

#ifndef HW1_ANALYZEREXCEPTION_H
#define HW1_ANALYZEREXCEPTION_H

#include "Exception.h"
#include <string>
#include <utility>

class AnalyzerException: public Exception {
public:
    explicit AnalyzerException(std::string message, int position):
        Exception(std::move(message)),
        position(position)
    {
        this->message += " Position " + std::to_string(position) + ".";
    }

    int getPostion() const;

private:
    int position;
};


#endif //HW1_ANALYZEREXCEPTION_H
