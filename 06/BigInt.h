//
// Created by admin2 on 25.11.2019.
//

#ifndef HW6_BIGINT_H
#define HW6_BIGINT_H

#include <string>
#include <limits>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <cmath>
#include "DynamicArray.h"

class BigInt {
public:
    using BigDigitType = unsigned int;
    using ComputationType = long long;
    static constexpr ComputationType BASE = 1000 * 1000 * 1000;
    static constexpr std::size_t NumberOfDecimalDigitsPerBigDigit = 9;

    explicit BigInt(int number = 0) {
        assignNumber(number);
    }

    BigInt(const std::string& number) {
        ParseFrom(number);
    }

    BigInt(const BigInt& number) = default;
    BigInt(BigInt&& number) = default;
    ~BigInt() = default;
    BigInt& operator = (const BigInt& number) = default;
    BigInt& operator = (BigInt&& number) = default;
    BigInt& operator = (int number) {
        assignNumber(number);
        return *this;
    }

    BigInt& operator += (const BigInt& number);

    BigInt& operator -= (const BigInt& number);

    BigInt operator + (const BigInt& number) const& {
        BigInt result(*this);
        result += number;
        return result;
    }

    BigInt operator + (const BigInt& number) & {
        BigInt result(*this);
        result += number;
        return result;
    }

    BigInt&& operator + (const BigInt& number) && {
        return std::move( *this += number );
    }

    BigInt operator - (const BigInt& number) const& {
        BigInt result(*this);
        result -= number;
        return result;
    }

    BigInt operator - (const BigInt& number) & {
        BigInt result(*this);
        result -= number;
        return result;
    }

    BigInt&& operator - (const BigInt& number) && {
        return std::move( *this -= number );
    }

    BigInt operator + (int number) {
        return *this + BigInt(number);
    }

    BigInt operator + (int number) const {
        return *this + BigInt(number);
    }

    BigInt operator - (int number) const {
        return *this - BigInt(number);
    }

    BigInt& operator += (int number) {
        *this = *this + number;
        return *this;
    }

    BigInt& operator -= (int number) {
        *this = *this - number;
        return *this;
    }

    BigInt operator - () const& {
        BigInt result = *this;
        if (!result.IsZero())
            result.isNotNegative = !result.isNotNegative;
        return result;
    }

    BigInt operator - () & {
        BigInt result = *this;
        if (!result.IsZero())
            result.isNotNegative = !result.isNotNegative;
        return result;
    }

    BigInt&& operator - () && {
        if (!IsZero())
            isNotNegative = !isNotNegative;
        return std::move(*this);
    }

    bool operator == (const BigInt& number) const {
        return Compare(*this, number) == 0;
    }

    bool operator == (int number) const {
        return *this == BigInt(number);
    }

    bool operator != (const BigInt& number) const {
        return Compare(*this, number) != 0;
    }

    bool operator != (int number) const {
        return *this != BigInt(number);
    }

    bool operator < (const BigInt& number) const {
        return Compare(*this, number) < 0;
    }

    bool operator < (int number) const {
        return *this < BigInt(number);
    }

    bool operator > (const BigInt& number) const {
        return Compare(*this, number) > 0;
    }

    bool operator > (int number) const {
        return *this > BigInt(number);
    }

    bool operator <= (const BigInt& number) const {
        return Compare(*this, number) <= 0;
    }

    bool operator <= (int number) const {
        return *this <= BigInt(number);
    }

    bool operator >= (const BigInt& number) const {
        return Compare(*this, number) >= 0;
    }

    bool operator >= (int number) const {
        return *this >= BigInt(number);
    }

    [[nodiscard]] bool IsZero() const {
        return digits.Size() == 1 && digits[0] == 0 && isNotNegative;
    }

    static int Compare(const BigInt& a, const BigInt& b);

    [[nodiscard]] std::string ToString() const;

    void ParseFrom(const std::string& number);

    void DebugPrint();

private:
    bool isNotNegative;
    DynamicArray<BigDigitType> digits;

    BigInt(DynamicArray<BigDigitType> digits, bool isNotNegative):
        digits(std::move(digits)),
        isNotNegative(isNotNegative)
    {
    }

    void assignNumber(int number);

    int compareDigits(const DynamicArray<BigDigitType>& digitsA, const DynamicArray<BigDigitType>& digitsB) const;


    void add(DynamicArray<BigDigitType>& digitsA, const DynamicArray<BigDigitType>& digitsB) const;

    void subtract(DynamicArray<BigDigitType>& digitsA, const DynamicArray<BigDigitType>& digitsB, bool& isNotNegative) const;

};

std::istream& operator >> (std::istream& inp, BigInt& number);

std::ostream& operator << (std::ostream& out, const BigInt& number);

BigInt operator + (int a, const BigInt& b);

BigInt&& operator + (int a, BigInt&& b);

BigInt operator - (int a, const BigInt& b);

BigInt&& operator - (int a, BigInt&& b);

bool operator == (int a, const BigInt& b);

bool operator != (int a, const BigInt& b);

bool operator < (int a, const BigInt& b);

bool operator > (int a, const BigInt& b);

bool operator <= (int a, const BigInt& b);

bool operator >= (int a, const BigInt& b);


#endif //HW6_BIGINT_H
