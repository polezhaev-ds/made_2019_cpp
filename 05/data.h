//
// Created by admin2 on 17.11.2019.
//

#pragma once

#include <cstdint>
#include "serialization.h"

struct Data {
    std::uint64_t a;
    bool b;
    std::uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }

    template <class Serializer>
    Error serialize(Serializer& serializer) const {
        return serializer(a, b, c);
    }

    bool operator == (const Data& data) const {
        return a == data.a && b == data.b && c == data.c;
    }

    bool operator != (const Data& data) const {
        return !(*this == data);
    }

};