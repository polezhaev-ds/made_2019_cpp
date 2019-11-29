//
// Created by admin2 on 07.11.2019.
//

#ifndef HW3_TOKENINFO_H
#define HW3_TOKENINFO_H

#include <iostream>
#include <string>
#include <cstddef>

class TokenInfo {
public:
    TokenInfo(std::string token, std::size_t position):
            token(std::move(token)),
            position(position)
    {
    }

    bool operator == (const TokenInfo& obj) const;

    bool operator != (const TokenInfo& obj) const;

    [[nodiscard]] const std::string& getToken() const;

    [[nodiscard]] std::size_t getPosition() const;

private:
    std::string token;
    std::size_t position;

};

std::ostream& operator << (std::ostream& out, const TokenInfo& ti);


#endif //HW3_TOKENINFO_H
