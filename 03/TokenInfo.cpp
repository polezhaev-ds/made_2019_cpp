//
// Created by admin2 on 07.11.2019.
//

#include "TokenInfo.h"

std::ostream &operator<<(std::ostream &out, const TokenInfo &ti) {
    return out << "( \"" << ti.getToken() << "\", " << ti.getPosition() << " )";
}

const std::string &TokenInfo::getToken() const {
    return token;
}

std::size_t TokenInfo::getPosition() const {
    return position;
}

bool TokenInfo::operator==(const TokenInfo &obj) const {
    return token == obj.token && position == obj.position;
}

bool TokenInfo::operator!=(const TokenInfo &obj) const {
    return !(*this == obj);
}
