//
// Created by admin2 on 17.11.2019.
//

#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <utility>
#include "errors.h"


class Deserializer
{
public:
    explicit Deserializer(std::istream& in)
            : in_(in)
    {
    }

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::istream& in_;

    [[nodiscard]] bool isUnsignedInteger(const std::string& str) const {
        for (char ch : str)
            if (!std::isdigit(ch))
                return false;
        return true;
    }

    Error load(bool& value) {
        std::string text;

        in_ >> text;

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error load(std::uint64_t& value) {
        std::string text;

        in_ >> text;

        try {
            if (!isUnsignedInteger(text))
                throw std::invalid_argument("Invalid unsigned integer format");
            value = std::stoull(text);
        }
        catch (const std::exception& e)
        {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    template <class T>
    Error process(T&& value) {
        return load(value);
    }

    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args) {
        Error error = load(value);
        if (error == Error::NoError) {
            return process(std::forward<ArgsT>(args)...);
        }
        return error;
    }

};

