//
// Created by admin2 on 17.11.2019.
//

#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include "errors.h"

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
            : out_(out)
    {
    }

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::ostream& out_;

    Error save(bool value) {
        if (value)
            out_ << "true";
        else
            out_ << "false";
        return Error::NoError;
    }

    Error save(std::uint64_t value) {
        out_ << value;
        return Error::NoError;
    }

    template <class T>
    Error process(T&& value) {
        return save(value);
    }

    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args) {
        Error error = save(value);
        if (error == Error::NoError) {
            out_ << Separator;
            return process(std::forward<ArgsT>(args)...);
        }
        return error;
    }

};
