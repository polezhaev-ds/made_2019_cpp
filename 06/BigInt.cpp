//
// Created by admin2 on 25.11.2019.
//

#include "BigInt.h"

BigInt &BigInt::operator += (const BigInt& number) {
    if (isNotNegative && !number.isNotNegative) {
        subtract(digits, number.digits, isNotNegative);
    } else if (!isNotNegative && number.isNotNegative) {
        subtract(digits, number.digits, isNotNegative);
        if (!IsZero())
            isNotNegative = !isNotNegative;
    } else {
        add(digits, number.digits);
    }
    return *this;
}

BigInt &BigInt::operator -= (const BigInt& number) {
    if (isNotNegative && !number.isNotNegative) {
        add(digits, number.digits);
    } else if (!isNotNegative && number.isNotNegative) {
        add(digits, number.digits);
    } else {
        bool prevIsNotNegative = isNotNegative;
        subtract(digits, number.digits, isNotNegative);
        if (!prevIsNotNegative && !IsZero())
            isNotNegative = !isNotNegative;
    }
    return *this;
}

int BigInt::Compare(const BigInt& a, const BigInt& b) {
    if (!a.isNotNegative && b.isNotNegative)
        return -1;
    if (a.isNotNegative && !b.isNotNegative)
        return 1;
    if (a.isNotNegative) {
        if (a.digits.Size() < b.digits.Size())
            return -1;
        else if (a.digits.Size() > b.digits.Size())
            return 1;
    } else {
        if (a.digits.Size() < b.digits.Size())
            return 1;
        else if (a.digits.Size() > b.digits.Size())
            return -1;
    }
    for (std::size_t i = a.digits.Size() - 1; i >= 0 && i < a.digits.Size(); i--)
        if (a.digits[i] < b.digits[i]) {
            if (a.isNotNegative)
                return -1;
            return 1;
        } else if (a.digits[i] > b.digits[i]) {
            if (a.isNotNegative)
                return 1;
            return -1;
        }
    return 0;
}

std::string BigInt::ToString() const {

    if (IsZero())
        return "0";

    std::string result;
    if (!isNotNegative)
        result = "-";

    for (std::size_t i = digits.Size() - 1; i >= 0 && i < digits.Size(); i--) {
        std::size_t numberOfDecimalDigitsForCurrentValue = digits[i] == 0 ? 1 : std::floor(std::log10(digits[i]) + 1);
        if (i != digits.Size() - 1)
            result += std::string( NumberOfDecimalDigitsPerBigDigit - numberOfDecimalDigitsForCurrentValue, '0');
        result += std::to_string(digits[i]);
    }

    return result;
}

void BigInt::ParseFrom(const std::string& number) {
    const char* ARGUMENT_IS_NOT_VALID_MESSAGE = "Argument number is not a valid string representation of big integer!";
    digits.Clear();

    //Check if number is empty
    if (number.empty())
        throw std::invalid_argument(ARGUMENT_IS_NOT_VALID_MESSAGE);

    //Skip leading spaces
    std::size_t numberStartIndex = 0;
    while (numberStartIndex < number.size() && std::isspace(number[numberStartIndex]))
        numberStartIndex++;

    //Skip trailing spaces
    std::size_t numberEndIndex = number.size() - 1;
    while (numberEndIndex >= 0 && std::isspace(number[numberEndIndex]))
        numberEndIndex--;

    //Check validity of a number sign
    if (number[numberStartIndex] != '-' && number[numberStartIndex] != '+' && !std::isdigit(number[numberStartIndex]))
        throw std::invalid_argument(ARGUMENT_IS_NOT_VALID_MESSAGE);

    isNotNegative = number[numberStartIndex] == '+' || std::isdigit(number[numberStartIndex]);

    //Skip a sign symbol
    if (number[numberStartIndex] == '-' || number[numberStartIndex] == '+')
        numberStartIndex++;

    //Skip possible spaces after sign symbol
    while (numberStartIndex < numberEndIndex && std::isspace(number[numberStartIndex]))
        numberStartIndex++;

    //Check if number has no symbols
    if (numberStartIndex > numberEndIndex)
        throw std::invalid_argument(ARGUMENT_IS_NOT_VALID_MESSAGE);

    //Check that all symbols of number are digits
    for (std::size_t i = numberStartIndex; i <= numberEndIndex; i++) {
        if (!std::isdigit(number[i]))
            throw std::invalid_argument(ARGUMENT_IS_NOT_VALID_MESSAGE);
    }

    //Skip possible leading zeros
    while (numberStartIndex < numberEndIndex && number[numberStartIndex] == '0')
        numberStartIndex++;

    ComputationType currentDigit = 0;
    std::size_t nDecimalDigits = 0;
    ComputationType powerOfTen = 1;
    for (std::size_t i = numberEndIndex; i >= numberStartIndex && i <= numberEndIndex; i--) {
        currentDigit = currentDigit + (number[i] - '0') * powerOfTen;
        nDecimalDigits++;
        powerOfTen *= 10;
        if (nDecimalDigits == NumberOfDecimalDigitsPerBigDigit) {
            digits.Append(currentDigit);
            currentDigit = 0;
            nDecimalDigits = 0;
            powerOfTen = 1;
        }
    }
    if (currentDigit > 0)
        digits.Append(currentDigit);

    if (currentDigit == 0 && digits.Size() == 0) { //For the case of zero with any sign
        digits.Append(currentDigit);
        isNotNegative = true;
    }
}

void BigInt::DebugPrint() {
    std::cout << isNotNegative;
    for (std::size_t i = 0; i < digits.Size(); i++)
        std::cout << " " << digits[i];
    std::cout << std::endl;
}

void BigInt::assignNumber(int number) {
    digits.Clear();
    isNotNegative = number >= 0;
    if (number == 0) {
        digits.Append(0);
        return;
    }

    ComputationType absNumber = std::abs((ComputationType)number);
    while (absNumber > 0) {
        digits.Append(absNumber % BASE);
        absNumber /= BASE;
    }
}

int BigInt::compareDigits(const DynamicArray<BigDigitType>& digitsA, const DynamicArray<BigDigitType>& digitsB) const {
    if (digitsA.Size() < digitsB.Size())
        return -1;
    else if (digitsA.Size() > digitsB.Size())
        return 1;
    for (std::size_t i = digitsA.Size() - 1; i >= 0 && i < digitsA.Size(); i--)
        if (digitsA[i] < digitsB[i])
            return -1;
        else if (digitsA[i] > digitsB[i])
            return 1;
    return 0;
}

void BigInt::add(DynamicArray<BigDigitType>& digitsA, const DynamicArray<BigDigitType>& digitsB) const {
    size_t max_length = std::max(digitsA.Size(), digitsB.Size());
    ComputationType comp = 0;
    BigDigitType carry = 0;
    for (std::size_t i = 0; i < max_length; i++) {
        comp = carry;
        if (i < digitsA.Size())
            comp += digitsA[i];
        if (i < digitsB.Size())
            comp += digitsB[i];
        carry = comp >= BASE;
        if (comp >= BASE)
            comp -= BASE;
        if (i < digitsA.Size())
            digitsA[i] = comp;
        else
            digitsA.Append(comp);
    }
    if (carry > 0)
        digitsA.Append(carry);
}

void BigInt::subtract(DynamicArray<BigDigitType>& digitsA, const DynamicArray<BigDigitType>& digitsB,
                      bool& isNotNegative) const {
    bool isLess = compareDigits(digitsA, digitsB) < 0;
    isNotNegative = !isLess;

    ComputationType comp = 0;
    BigDigitType carry = 0;

    std::size_t max_size = std::max(digitsA.Size(), digitsB.Size());

    for (std::size_t i = 0; i < max_size; i++) {
        if (!isLess) {
            comp = (ComputationType)digitsA[i] - carry;
            if (i < digitsB.Size())
                comp -= digitsB[i];
        } else {
            comp = (ComputationType)digitsB[i] - carry;
            if (i < digitsA.Size())
                comp -= digitsA[i];
        }
        carry = comp < 0;
        if (comp < 0)
            comp += BASE;
        if (i < digitsA.Size())
            digitsA[i] = comp;
        else
            digitsA.Append(comp);
    }

    while (digitsA.Size() > 1 && digitsA[digitsA.Size() - 1] == 0)
        digitsA.DeleteLast();

    if (digitsA.Size() == 1 && digitsA[digitsA.Size() - 1] == 0)
        isNotNegative = true;
}


std::istream& operator >> (std::istream& inp, BigInt &number) {
    std::string word;
    inp >> word;
    number.ParseFrom(word);
    return inp;
}

std::ostream& operator << (std::ostream& out, const BigInt& number) {
    out << number.ToString();
    return out;
}

BigInt operator + (int a, const BigInt& b) {
    BigInt result(a);
    result += b;
    return result;
}

BigInt&& operator + (int a, BigInt&& b) {
    b += a;
    return std::move(b);
}

BigInt operator - (int a, const BigInt& b) {
    return BigInt(a) - b;
}

BigInt&& operator - (int a, BigInt&& b) {
    b -= a;
    return std::move(-b);
}

bool operator == (int a, const BigInt& b) {
    return BigInt(a) == b;
}

bool operator != (int a, const BigInt& b) {
    return BigInt(a) != b;
}

bool operator < (int a, const BigInt& b) {
    return BigInt(a) < b;
}

bool operator > (int a, const BigInt& b) {
    return BigInt(a) > b;
}

bool operator <= (int a, const BigInt& b) {
    return BigInt(a) <= b;
}

bool operator >= (int a, const BigInt& b) {
    return BigInt(a) >= b;
}
