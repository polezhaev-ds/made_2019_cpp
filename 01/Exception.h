//
// Created by admin2 on 28.10.2019.
//

#ifndef HW1_EXCEPTION_H
#define HW1_EXCEPTION_H

#include <string>

class Exception: public std::exception {
public:
    explicit Exception(std::string message):
        message(std::move(message))
    {
    }

    const std::string& getMessage() const;

    const char* what() const noexcept override;

protected:
    std::string message;
};


#endif //HW1_EXCEPTION_H
