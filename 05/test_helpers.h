//
// Created by admin2 on 18.11.2019.
//

#pragma once

#include "serialization.h"

template <class T>
struct SingleFieldData {
    T a;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a);
    }

    template <class Serializer>
    Error serialize(Serializer& serializer) const {
        return serializer(a);
    }

    bool operator == (const SingleFieldData& data) const {
        return a == data.a;
    }

    bool operator != (const SingleFieldData& data) const {
        return !(*this == data);
    }

};

