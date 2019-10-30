//
// Created by admin2 on 28.10.2019.
//


#ifndef HW1_TOKEN_H
#define HW1_TOKEN_H

#include <string>
#include <memory>

enum TokenType {
    None, OperationPlus, OperationMinus, OperationMultiply, OperationDivide,
    LeftBracket, RightBracket, Number
};

class Token {
public:
    explicit Token(TokenType type, int position) :
        type(type),
        position(position)
    {

    }

    TokenType getType() const;
    int getPosition() const;
    virtual std::string getValue() const;
    virtual std::string toString() const;

    #ifndef NDEBUG
    virtual std::string toDebugString() const;
    #endif

    static std::shared_ptr<Token> CreateToken(const std::string& tokenStr, int position);

private:
    TokenType type;
    int position;
};


#endif //HW1_TOKEN_H
