//
// Created by admin2 on 17.11.2019.
//

#define CATCH_CONFIG_MAIN

#include <string>
#include <sstream>
#include <type_traits>
#include "catch2/catch.hpp"
#include "BigInt.h"

std::string FormatNumber(const std::string& str) {
    std::string formatedStr;
    std::size_t endPosition = str.size() - 1;
    while (endPosition > 0 && std::isspace(str[endPosition]) )
        endPosition--;

    bool isDigitFound = false;
    bool isLeadingZero = true;
    for (std::size_t i = 0; i <= endPosition; i++)
        if (str[i] == '-' || (std::isspace(str[i]) && isDigitFound)) {
            formatedStr.push_back(str[i]);
        } else if (std::isdigit(str[i])) {
            if (!isLeadingZero || isLeadingZero && (str[i] != '0' || str[i] == '0' && i == endPosition)) {
                formatedStr.push_back(str[i]);
                isLeadingZero = false;
            }

            isDigitFound = true;

        }
    return formatedStr;
}

bool TestConstructorForInteger(int number) {
    BigInt x(number);

    return x.ToString() == std::to_string(number);
}

bool TestConstructorForString(const std::string& number) {
    BigInt x(number);

    std::string strToCompareWith = FormatNumber(number);

    return x.ToString() == strToCompareWith;
}

bool TestCopyConstructor(const std::string& number) {
    BigInt x(number);
    BigInt y = x;
    bool areEqual = x == y;
    x = x + 1;

    return x != y && areEqual;
}

bool TestMoveConstructor(const std::string& number) {
    BigInt x(number);
    BigInt y(std::move(x));
    BigInt z(number);

    return x != y && y == z;
}

template <typename T>
bool TestAssignmentOperator(T&& x) {
    BigInt y;
    y = x;
    bool areEqual = x == y;
    y = y + 1;

    return x != y && areEqual;
}

bool TestMoveAssignmentOperator(const std::string& number) {
    BigInt x(number);
    BigInt y;
    BigInt z(number);
    y = std::move(x);

    return x != y && y == z;
}

template <typename T1, typename T2>
bool TestAssignmentWithAdditionOperator(T1&& x, T2&& y, const std::string& expectedResult) {
    BigInt* px = &(x += y);
    BigInt result(expectedResult);

    return result == x && px == &x;
}

template <typename T1, typename T2>
bool TestAssignmentWithSubtractionOperator(T1&& x, T2&& y, const std::string& expectedResult) {
    BigInt* px = &(x -= y);
    BigInt result(expectedResult);

    return result == x && px == &x;
}

template <typename T1, typename T2>
bool TestAdditionOperator(T1&& x, T2&& y, const std::string& expectedResult) {
    BigInt z1 = x + y;
    BigInt z2 = y + x;
    BigInt result(expectedResult);

    return result == z1 && result == z2;
}

template <typename T1, typename T2>
bool TestSubtractionOperator(T1&& x, T2&& y, const std::string& expectedResult) {
    BigInt z1 = x - y;
    BigInt z2 = y - x;
    BigInt result(expectedResult);

    return result == z1 && 0 - result == z2;
}

bool TestUnaryMinusOperator(const std::string& operand, const std::string& expectedResult) {
    BigInt x(operand);
    BigInt y = -x;
    BigInt expected(expectedResult);

    return expected == y;
}


template <typename T1, typename T2>
bool TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(T1&& x, T2&& y) {
    return x == y && y == x && !(x != y) && !(y != x) && x <= y && y <= x && x >= y && y >= x &&
           !(x > y) && !(y > x) && !(x < y) && !(y < x);
}

template <typename T1, typename T2>
bool TestEqualityAndInequalityOperatorsForNotEqualNumbers(T1&& x, T2&& y) {
    return !(x == y) && !(y == x) && x != y && y != x;
}

template <typename T1, typename T2>
bool TestLessAndGreaterOperatorsForLessAndGreaterNumbers(T1&& lessNumber, T2&& greaterNumber) {
    return lessNumber < greaterNumber && !(greaterNumber < lessNumber) &&
           !(lessNumber > greaterNumber) && greaterNumber > lessNumber &&
           lessNumber <= greaterNumber && !(greaterNumber <= lessNumber) &&
           !(lessNumber >= greaterNumber) && greaterNumber >= lessNumber;
}

bool TestParseFromAndToStringMethods(const std::string& strNumber) {
    BigInt x;
    x.ParseFrom(strNumber);

    std::string strToCompareWith = FormatNumber(strNumber);

    return x.ToString() == strToCompareWith;
}

TEST_CASE( "BigInt's class constructors work correctly" ) {

    SECTION( "BigInt's constructor for short numbers works correctly" ) {
        CHECK( TestConstructorForInteger(123) );
        CHECK( TestConstructorForInteger(-123) );
        CHECK( TestConstructorForInteger(0) );
        CHECK( TestConstructorForInteger(std::numeric_limits<int>::max()) );
        CHECK( TestConstructorForInteger(std::numeric_limits<int>::min()) );
    }

    SECTION( "BigInt's constructor for strings works correctly" ) {
        CHECK( TestConstructorForString("0") );
        CHECK( TestConstructorForString("   0    ") );
        CHECK( TestConstructorForString("1231231231231231231234345345345345345345345345") );
        CHECK( TestConstructorForString("-1231231231231231231234345345345345345345345345") );
        CHECK( TestConstructorForString("+1231231231231231231234345345345345345345345345") );
        CHECK( TestConstructorForString(" +9999999999999999999999999999999999999999999999999999999999999 ") );
        CHECK( TestConstructorForString(" -9999999999999999999999999999999999999999999999999999999999999 ") );
    }

    SECTION( "BigInt's copy constructor works correctly" ) {
        CHECK( TestCopyConstructor("0") );
        CHECK( TestCopyConstructor("-1231231231231231231234345345345345345345345345") );
        CHECK( TestCopyConstructor("+1231231231231231231234345345345345345345345345") );
    }

    SECTION( "BigInt's move constructor works correctly" ) {
        CHECK( TestMoveConstructor("0") );
        CHECK( TestMoveConstructor("-1231231231231231231234345345345345345345345345") );
        CHECK( TestMoveConstructor("+1231231231231231231234345345345345345345345345") );
    }
}

TEST_CASE( "BigInt's assignment operators work correctly" ) {

    SECTION( "BigInt's assignment operator works correctly" ) {
        CHECK( TestAssignmentOperator(0) );
        CHECK( TestAssignmentOperator(1234567890) );
        CHECK( TestAssignmentOperator(-1234567890) );
        CHECK( TestAssignmentOperator(std::numeric_limits<int>::max()) );
        CHECK( TestAssignmentOperator(std::numeric_limits<int>::min()) );
        CHECK( TestAssignmentOperator(BigInt("0")) );
        CHECK( TestAssignmentOperator(BigInt("-1231231231231231231234345345345345345345345345")) );
        CHECK( TestAssignmentOperator(BigInt("+1231231231231231231234345345345345345345345345")) );
    }

    SECTION( "BigInt's move assignment operator works correctly" ) {
        CHECK( TestMoveAssignmentOperator("0") );
        CHECK( TestMoveAssignmentOperator("-1231231231231231231234345345345345345345345345") );
        CHECK( TestMoveAssignmentOperator("+1231231231231231231234345345345345345345345345") );
    }

    SECTION( "BigInt's assignment with addition operator works correctly for short numbers" ) {
        CHECK( TestAssignmentWithAdditionOperator(BigInt("234234234234234234234"), 99999, "234234234234234334233") );
        CHECK( TestAssignmentWithAdditionOperator(BigInt("234234234234234234234"), -99999, "234234234234234134235") );
        CHECK( TestAssignmentWithAdditionOperator(BigInt("1234567890"), -1234567890, "0") );
    }

    SECTION( "BigInt's assignment with addition operator works correctly for big numbers" ) {
        CHECK( TestAssignmentWithAdditionOperator(BigInt("234234234234234234234"), BigInt("99999999999999999999999999"),
            "100000234234234234234234233") );
        CHECK( TestAssignmentWithAdditionOperator(BigInt("234234234234234234234"), BigInt("-99999999999999999999999999"),
            "-99999765765765765765765765") );
        CHECK( TestAssignmentWithAdditionOperator(BigInt("234234234234234234234"), BigInt("-234234234234234234234"),
            "0") );
    }

    SECTION( "BigInt's assignment with subtraction operator works correctly for short numbers" ) {
        CHECK( TestAssignmentWithSubtractionOperator(BigInt("234234234234234234234"), 99999, "234234234234234134235") );
        CHECK( TestAssignmentWithSubtractionOperator(BigInt("234234234234234234234"), -99999, "234234234234234334233") );
        CHECK( TestAssignmentWithSubtractionOperator(BigInt("1234567890"), 1234567890, "0") );
    }

    SECTION( "BigInt's assignment with subtraction operator works correctly for big numbers" ) {
        CHECK( TestAssignmentWithSubtractionOperator(BigInt("234234234234234234234"), BigInt("99999999999999999999999999"),
            "-99999765765765765765765765") );
        CHECK( TestAssignmentWithSubtractionOperator(BigInt("234234234234234234234"), BigInt("-99999999999999999999999999"),
            "100000234234234234234234233") );
        CHECK( TestAssignmentWithSubtractionOperator(BigInt("234234234234234234234"), BigInt("234234234234234234234"),
            "0") );
    }

}

TEST_CASE( "BigInts's unary minus works correctly")
{
    SECTION( "BigInt's unary minus operator work correctly") {
        CHECK( TestUnaryMinusOperator("0", "0") );
        CHECK( TestUnaryMinusOperator("0", "-0") );
        CHECK( TestUnaryMinusOperator("-5", "5") );
        CHECK( TestUnaryMinusOperator("5", "-5") );
        CHECK( TestUnaryMinusOperator("545345345345345345345341299999999999", "-545345345345345345345341299999999999") );
    }
}

TEST_CASE( "BigInt's addition operators work correctly" ) {

    SECTION( "BigInt's addition operator with short numbers works correctly" ) {
        CHECK( TestAdditionOperator(BigInt("0"), 0, "0") );
        CHECK( TestAdditionOperator(BigInt("0"), 5, "5") );
        CHECK( TestAdditionOperator(BigInt("0"), -5, "-5") );
        CHECK( TestAdditionOperator(BigInt("5"), -5, "0") );
        CHECK( TestAdditionOperator(BigInt("5"), 5, "10") );
        CHECK( TestAdditionOperator(BigInt("-5"), 5, "0") );
        CHECK( TestAdditionOperator(BigInt("-5"), -5, "-10") );
        CHECK( TestAdditionOperator(BigInt("12345"), 905, "13250") );
        CHECK( TestAdditionOperator(BigInt("-123456"), std::numeric_limits<int>::max(), "2147360191") );
        CHECK( TestAdditionOperator(BigInt("123456"), std::numeric_limits<int>::max(), "2147607103") );
        CHECK( TestAdditionOperator(BigInt("123456"), std::numeric_limits<int>::min(), "-2147360192") );
        CHECK( TestAdditionOperator(BigInt("-123456"), std::numeric_limits<int>::min(), "-2147607104") );
        CHECK( TestAdditionOperator(BigInt("123456789012345678901234567890"), std::numeric_limits<int>::min(),
            "123456789012345678899087084242") );
        CHECK( TestAdditionOperator(BigInt("-123456789012345678901234567890"), std::numeric_limits<int>::min(),
            "-123456789012345678903382051538") );
        CHECK( TestAdditionOperator(BigInt("123456789012345678901234567890"), std::numeric_limits<int>::max(),
            "123456789012345678903382051537") );
        CHECK( TestAdditionOperator(BigInt("-123456789012345678901234567890"), std::numeric_limits<int>::max(),
            "-123456789012345678899087084243") );
    }

    SECTION( "BigInt's addition operator with big numbers works correctly" ) {
        CHECK( TestAdditionOperator(BigInt("0"), BigInt("0"), "0") );
        CHECK( TestAdditionOperator(BigInt("-5"), BigInt("5"), "0") );
        CHECK( TestAdditionOperator(BigInt("5"), BigInt("-5"), "0") );
        CHECK( TestAdditionOperator(BigInt("123456789012345678901234567890"),
            BigInt("-123456789012345678901234567890"), "0") );
        CHECK( TestAdditionOperator(BigInt("123500"), BigInt("-500"), "123000") );
        CHECK( TestAdditionOperator(BigInt("123500"), BigInt("-123001"), "499") );
        CHECK( TestAdditionOperator(BigInt("145345345345344534534534534534534587979"),
            BigInt("234234234242342342334566546414234234343"), "379579579587686876869101080948768822322") );
        CHECK( TestAdditionOperator(BigInt("145345345345344534534534534534534587979"),
            BigInt("-234234234242342342334566546414234234343"), "-88888888896997807800032011879699646364") );
        CHECK( TestAdditionOperator(BigInt("-145345345345344534534534534534534587979"),
            BigInt("234234234242342342334566546414234234343"), "88888888896997807800032011879699646364") );
        CHECK( TestAdditionOperator(BigInt("9999999999999999999999999999999999999"),
            BigInt("123"), "10000000000000000000000000000000000122") );
        CHECK( TestAdditionOperator(BigInt("9999999999999999999999999999999999999"),
            BigInt("9999999999999999"), "10000000000000000000009999999999999998") );
        CHECK( TestAdditionOperator(BigInt("100000000002342342342364000000000000000000"),
            BigInt("-9999999999999999"), "100000000002342342342363990000000000000001") );
    }
}

TEST_CASE( "BigInt's subtraction operators work correctly" ) {

    SECTION( "BigInt's subtraction operator with short numbers works correctly" ) {
        CHECK( TestSubtractionOperator(BigInt("0"), 0, "0") );
        CHECK( TestSubtractionOperator(BigInt("0"), 5, "-5") );
        CHECK( TestSubtractionOperator(BigInt("0"), -5, "5") );
        CHECK( TestSubtractionOperator(BigInt("5"), 5, "0") );
        CHECK( TestSubtractionOperator(BigInt("-5"), 5, "-10") );
        CHECK( TestSubtractionOperator(BigInt("-5"), -5, "0") );
        CHECK( TestSubtractionOperator(BigInt("12345"), 905, "11440") );
        CHECK( TestSubtractionOperator(BigInt("-123456"), std::numeric_limits<int>::max(), "-2147607103") );
        CHECK( TestSubtractionOperator(BigInt("123456"), std::numeric_limits<int>::max(), "-2147360191") );
        CHECK( TestSubtractionOperator(BigInt("123456"), std::numeric_limits<int>::min(), "2147607104") );
        CHECK( TestSubtractionOperator(BigInt("-123456"), std::numeric_limits<int>::min(), "2147360192") );
        CHECK( TestSubtractionOperator(BigInt("123456789012345678901234567890"), std::numeric_limits<int>::min(),
            "123456789012345678903382051538") );
        CHECK( TestSubtractionOperator(BigInt("-123456789012345678901234567890"), std::numeric_limits<int>::min(),
            "-123456789012345678899087084242") );
        CHECK( TestSubtractionOperator(BigInt("123456789012345678901234567890"), std::numeric_limits<int>::max(),
            "123456789012345678899087084243") );
        CHECK( TestSubtractionOperator(BigInt("-123456789012345678901234567890"), std::numeric_limits<int>::max(),
            "-123456789012345678903382051537") );
    }

    SECTION( "BigInt's addition operator with big numbers works correctly" ) {
        CHECK( TestSubtractionOperator(BigInt("0"), BigInt("0"), "0") );
        CHECK( TestSubtractionOperator(BigInt("-5"), BigInt("5"), "-10") );
        CHECK( TestSubtractionOperator(BigInt("5"), BigInt("-5"), "10") );
        CHECK( TestSubtractionOperator(BigInt("123456789012345678901234567890"),
            BigInt("123456789012345678901234567890"), "0") );
        CHECK( TestSubtractionOperator(BigInt("123456789012345678901234567890"),
            BigInt("-123456789012345678901234567890"), "246913578024691357802469135780") );
        CHECK( TestSubtractionOperator(BigInt("123500"), BigInt("-500"), "124000") );
        CHECK( TestSubtractionOperator(BigInt("123500"), BigInt("-123001"), "246501") );
        CHECK( TestSubtractionOperator(BigInt("145345345345344534534534534534534587979"),
            BigInt("234234234242342342334566546414234234343"), "-88888888896997807800032011879699646364") );
        CHECK( TestSubtractionOperator(BigInt("145345345345344534534534534534534587979"),
            BigInt("-234234234242342342334566546414234234343"), "379579579587686876869101080948768822322") );
        CHECK( TestSubtractionOperator(BigInt("-145345345345344534534534534534534587979"),
            BigInt("234234234242342342334566546414234234343"), "-379579579587686876869101080948768822322") );
        CHECK( TestSubtractionOperator(BigInt("9999999999999999999999999999999999999"),
            BigInt("123"), "9999999999999999999999999999999999876") );
        CHECK( TestSubtractionOperator(BigInt("9999999999999999999999999999999999999"),
            BigInt("9999999999999999"), "9999999999999999999990000000000000000") );
        CHECK( TestSubtractionOperator(BigInt("100000000002342342342364000000000000000000"),
            BigInt("9999999999999999"), "100000000002342342342363990000000000000001") );
    }
}

TEST_CASE( "BigInt's comparison operators work correctly" ) {

    SECTION( "BigInt's ==, !=, <=, =>, <, > operators work correctly for equal numbers" )
    {
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("0"), 0) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("-0"), 0) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("+0"), 0) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("5"), 5) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("+5"), 5) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("-5"), -5) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("-999999999"), -999999999) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("1234567890"), 1234567890) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("2147483647"),
                std::numeric_limits<int>::max()) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("+2147483647"),
                std::numeric_limits<int>::max()) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("-2147483648"),
                std::numeric_limits<int>::min()) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(BigInt("0"), BigInt("0")) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(
                BigInt("3534534534599242042394234235434534"), BigInt("3534534534599242042394234235434534")) );
        CHECK( TestComparisonOperatorsWithEqualityOrInequalityForEqualNumbers(
                BigInt("-3534534534599242042394234235434534"), BigInt("-3534534534599242042394234235434534")) );
    }

    SECTION( "BigInt's ==, != operators work correctly for not equal numbers" )
    {
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("0"), 5) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("-0"), 5) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("+0"), 5) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("-5"), 5) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("+5"), -5) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("-5"), 55) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("-9999999999"), -9999999998) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("-9999999999"), -8999999999) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("9999999999"), 8999999999) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("9999999999"), 8999999999) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("9999999999"), 9999999998) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("9999999999"), 999999999) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("1234567890"), 1234567891) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("1234567890"), 2234567890) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("1234567890"), 123456789) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("2147483648"),
            std::numeric_limits<int>::max()) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("+2147483648"),
            std::numeric_limits<int>::max()) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(BigInt("-2147483647"),
            std::numeric_limits<int>::min()) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(
            BigInt("534534534599242042394234235434534"), BigInt("3534534534599242042394234235434534")) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(
            BigInt("4534534534599242042394234235434534"), BigInt("3534534534599242042394234235434534")) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(
            BigInt("3534534534599242042394234235434533"), BigInt("3534534534599242042394234235434534")) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(
            BigInt("-3534534534599242042394234235434533"), BigInt("-3534534534599242042394234235434534")) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(
            BigInt("-4534534534599242042394234235434534"), BigInt("-3534534534599242042394234235434534")) );
        CHECK( TestEqualityAndInequalityOperatorsForNotEqualNumbers(
            BigInt("-4599242042394234235434534"), BigInt("-3534534534599242042394234235434534")) );
    }


    SECTION( "BigInt's comparison operators for less and greater values (in such order) works correctly" ) {
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("0"), 1) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("-1"), 0) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("-1"), 1) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("1421234"), 2141231122) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("-1234567890"), 1234567890) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("-1234567891"), -1234567890) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("1234567890"), 1234567891) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("-2234567890"), -1234567890) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("1234567890"), 2134567890) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("-2147483648"),
            std::numeric_limits<int>::max()) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("2342354345346342342342342363"),
            BigInt("2342354345346342342342342365")) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("-2342354345346342342342342363"),
           BigInt("2342354345346342342342342363")) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("1342354345346342342342342363"),
           BigInt("2342354345346342342342342363")) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("46342342342342363"),
            BigInt("2342354345346342342342342363")) );
        CHECK( TestLessAndGreaterOperatorsForLessAndGreaterNumbers(BigInt("2342354345346342342342342363"),
            BigInt("23423345235358979725234523453453453453453")) );
    }

}

TEST_CASE( "BigInt's other methods and complex arithmetic expressions are correct" ) {

    SECTION( "BigInt's parser and string serializer work correctly" ) {
        CHECK( TestParseFromAndToStringMethods("0") );
        CHECK( TestParseFromAndToStringMethods("00") );
        CHECK( TestParseFromAndToStringMethods(" 0 ") );
        CHECK( TestParseFromAndToStringMethods("-123") );
        CHECK( TestParseFromAndToStringMethods("+123") );
        CHECK( TestParseFromAndToStringMethods("-0123") );
        CHECK( TestParseFromAndToStringMethods("+0123") );
        CHECK( TestParseFromAndToStringMethods("-000000000123") );
        CHECK( TestParseFromAndToStringMethods("+0000000000000000000123") );
        CHECK( TestParseFromAndToStringMethods(" -   123 ") );
        CHECK( TestParseFromAndToStringMethods("    123 ") );
        CHECK( TestParseFromAndToStringMethods(" +   123 ") );
        CHECK( TestParseFromAndToStringMethods("    123 ") );
        CHECK( TestParseFromAndToStringMethods("    2342346456456346456345634562353456456354563456135123 ") );
        CHECK( TestParseFromAndToStringMethods("\t\t-\t\t2342346456456346456345634562353456456354563456135123\t") );
        CHECK( TestParseFromAndToStringMethods("\t\t+\t\t2342346456456346456345634562353456456354563456135123\t") );
        CHECK( TestParseFromAndToStringMethods("\t\t-\t\t2342346456456346456345634562353456456354563456135123\t") );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods(""), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("                  "), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("+"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("-"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods(" +   "), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods(" - "), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("++123"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("-+123"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("+-123"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("--123"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods(" - + 123"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods(" - 123 234"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("-123 345"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("123 213423"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("++0"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("+-0"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("--0"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("-+0"), std::invalid_argument );
        CHECK_THROWS_AS( TestParseFromAndToStringMethods("0 0"), std::invalid_argument );
    }

    SECTION("Complex arithmetic expressions work correctly") {
        BigInt x("234234234234234234324");
        BigInt y("23423423432412222222222");
        BigInt z("4234234234343432423424234");
        BigInt u("123123123123");
        CHECK( x + y + z + u == BigInt("4257891892010202003003903") );
        CHECK( BigInt("234234234234234234324") + BigInt("23423423432412222222222") + BigInt("4234234234343432423424234") +
            BigInt("123123123123") == BigInt("4257891892010202003003903") );
        CHECK( BigInt("234234234234234234324") - BigInt("23423423432412222222222") - BigInt("4234234234343432423424234") -
            BigInt("123123123123") == BigInt("-4257423423541733534535255") );
        CHECK( x - y - z - u == BigInt("-4257423423541733534535255") );
        CHECK( x + y - z + u == BigInt("-4210576576676662843844565") );
        CHECK( x + 108889 + 289999 - 234234 == BigInt("234234234234234398978"));
        CHECK( x + 23423423 + y + 234324234 + z == BigInt("4257891892010079137628437") );
        CHECK( BigInt("234234234234234234324") + 23423423 + BigInt("23423423432412222222222") + 234324234 +
            BigInt("4234234234343432423424234") == BigInt("4257891892010079137628437") );
        CHECK( 23423423 + BigInt("234234234234234234324") + BigInt("23423423432412222222222") + 234324234 +
            BigInt("4234234234343432423424234") == BigInt("4257891892010079137628437") );
        CHECK( 23423423 + x + y + 234324234 + z == BigInt("4257891892010079137628437") );
        CHECK( (-(-x) == x) );
        CHECK( (-(-BigInt("4257891892010079137628437")) == BigInt("4257891892010079137628437")) );
    }

    SECTION("Complex arithmetic expressions work correctly for constant objects") {
        const BigInt x("234234234234234234324");
        const BigInt y("23423423432412222222222");
        const BigInt z("4234234234343432423424234");
        const BigInt u("123123123123");
        const BigInt a("4257891892010202003003903");
        const BigInt b("-4257423423541733534535255");
        const BigInt c("4257891892010079137628437");
        CHECK( x + y + z + u == a );
        CHECK( x - y - z - u == b );
        CHECK( x + 23423423 + y + 234324234 + z == c );
        CHECK( 23423423 + x + y + 234324234 + z == c );
        CHECK( 23423423 - x + y == 23423423 + y - x );
        CHECK( -(-x) + -(-y) == x + y );
    }

}
