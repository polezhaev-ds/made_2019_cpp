//
// Created by admin2 on 29.10.2019.
//

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "Parser.h"

TEST_CASE( "parser calculates expressions correctly", "[Parser]" ) {

    SECTION( "the simplest expressions are calculated correctly" ) {
        CHECK( ParseAndCompute("0") == 0 );
        CHECK( ParseAndCompute("5555") == 5555 );
        CHECK( ParseAndCompute("-1000") == -1000 );
        CHECK( ParseAndCompute("(400)") == 400 );
        CHECK( ParseAndCompute("((((123))))") == 123 );
    }

    SECTION( "expressions with one operation are calculated correctly" ) {
        CHECK( ParseAndCompute("25+10") == 35 );
        CHECK( ParseAndCompute("25-10") == 15 );
        CHECK( ParseAndCompute("25*10") == 250 );
        CHECK( ParseAndCompute("25/10") == 2 );
        CHECK( ParseAndCompute("-25") == -25 );
    }

    SECTION( "priorites of operations are taken into account and they are calculcated correctly" ) {
        CHECK( ParseAndCompute("25+10+30") == 65 );
        CHECK( ParseAndCompute("(25+10)+30") == 65 );
        CHECK( ParseAndCompute("-(25+10)+30") == -5 );
        CHECK( ParseAndCompute("25+(10+30)") == 65 );
        CHECK( ParseAndCompute("25-10-30") == -15 );
        CHECK( ParseAndCompute("(25-10)-30") == -15 );
        CHECK( ParseAndCompute("25-(10-30)") == 45 );
        CHECK( ParseAndCompute("25+5-10") == 20 );
        CHECK( ParseAndCompute("(25+5)-10") == 20 );
        CHECK( ParseAndCompute("25+(5-10)") == 20 );
        CHECK( ParseAndCompute("25-10+8") == 23 );
        CHECK( ParseAndCompute("(25-10)+8") == 23 );
        CHECK( ParseAndCompute("25-(10+8)") == 7 );
        CHECK( ParseAndCompute("25/5/5") == 1 );
        CHECK( ParseAndCompute("(25/5)/5") == 1 );
        CHECK( ParseAndCompute("25/(5/5)") == 25 );
        CHECK( ParseAndCompute("25/2*10") == 120 );
        CHECK( ParseAndCompute("(25/2)*10") == 120 );
        CHECK( ParseAndCompute("25/(2*10)") == 1 );
        CHECK( ParseAndCompute("25*10/2") == 125 );
        CHECK( ParseAndCompute("(25*10)/2") == 125 );
        CHECK( ParseAndCompute("25*(10/2)") == 125 );
        CHECK( ParseAndCompute("-25+-10") == -35 );
        CHECK( ParseAndCompute("-25+10") == -15 );
        CHECK( ParseAndCompute("25+-10") == 15 );
        CHECK( ParseAndCompute("25--10") == 35 );
        CHECK( ParseAndCompute("25+10*3") == 55 );
        CHECK( ParseAndCompute("(25+10)*3") == 105 );
        CHECK( ParseAndCompute("10-70/25") == 8 );
        CHECK( ParseAndCompute("(10-70)/25") == -2 );
        CHECK( ParseAndCompute("(10-70)/-25") == 2 );
        CHECK( ParseAndCompute("25--10") == 35 );
        CHECK( ParseAndCompute("25-10*(5+7/(2-8))") == -15 );
        CHECK( ParseAndCompute("25-(5+7/(2-8))*10") == -15 );
        CHECK( ParseAndCompute("25-(5+7/2-8)*10") == 25 );
    }

    SECTION( "complex expression are calculated correctly" ) {
        CHECK( ParseAndCompute("12345234+234*(3234--234) / 23*(2334+112)*-1") == -73956984 );
        CHECK( ParseAndCompute("(1534534534/(342+343*4)/(333-2*10)+5345*11)*122") == 7521910 );
        CHECK( ParseAndCompute("-(-(33*123 + 34234 /(11+88*(3-4))))+23423*(-111-234)") == -8077320 );
        CHECK( ParseAndCompute("43545*2*122*22/232-234*(234+2 *(234+2342)*2)") == -1458351 );
    }

    SECTION( "expressions with spaces" ) {
        CHECK( ParseAndCompute("  25 + 10 + 30 ") == 65 );
        CHECK( ParseAndCompute(" ( 25 + 10 ) + 30 ") == 65 );
        CHECK( ParseAndCompute("           25   +(10+30)") == 65 );
        CHECK( ParseAndCompute("25-10-30   ") == -15 );
        CHECK( ParseAndCompute("(25-10)      -30") == -15 );
        CHECK( ParseAndCompute("25 - (10 - 30)") == 45 );
        CHECK( ParseAndCompute("\t25\t+\t5-10") == 20 );
        CHECK( ParseAndCompute("\t(25+5)-10\t") == 20 );
        CHECK( ParseAndCompute("\t25\t+\t(\t5\t-\t10\t)\t") == 20 );
    }
}

TEST_CASE( "parser correctly shows errors for expressions", "[Parser]" ) {
    const char* divisonByZeroMessage = "Error! Division by zero!";

    SECTION( "division by zero" ) {
        CHECK_THROWS_WITH( ParseAndCompute("5/0"), Catch::Contains(divisonByZeroMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("0/0"), Catch::Contains(divisonByZeroMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("10/(5-5)"), Catch::Contains(divisonByZeroMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("10/(5-2*2 -1)"), Catch::Contains(divisonByZeroMessage) );
    }

    const char* incompleteExpressionMessage = "Error! Incomplete expression!";
    SECTION( "incomplete expressions" ) {
        CHECK_THROWS_WITH( ParseAndCompute("("), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("-"), Catch::Contains(incompleteExpressionMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("123+"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123-"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123*"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123/"), Catch::Contains(incompleteExpressionMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("123+5+"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123+5-"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123+5*"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123+5/"), Catch::Contains(incompleteExpressionMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("123-5+"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123-5-"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123-5*"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123-5/"), Catch::Contains(incompleteExpressionMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("123*6+"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123*6-"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123*6*"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123*6/"), Catch::Contains(incompleteExpressionMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("123/2+"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123/2-"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123/2*"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123/2/"), Catch::Contains(incompleteExpressionMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("123-5*(7-8)/"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(20 - 4 * 8) +"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(20 - 4 * 8) -"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(20 - 4 * 8) *"), Catch::Contains(incompleteExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(20 - 4 * 8) /"), Catch::Contains(incompleteExpressionMessage) );
    }

    const char* invalidTokenMessage = "Error! Invalid token!";
    SECTION( "invalid token" ) {
        CHECK_THROWS_WITH( ParseAndCompute("+"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("*"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("/"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("-+"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("--"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("-*"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("-/"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("2++"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2+*"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2+/"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("2-+"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2-*"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2-/"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("2*+"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2**"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2*/"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("2/+"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2/*"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2//"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("-+6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("--6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("-*6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("-/6+70"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("2++6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2+*6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2+/6+70"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("2-+6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2-*6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2-/6+70"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("2*+6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2**6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2*/6+70"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("2/+6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2/*6+70"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2//6+70"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("2/+6-4"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2/*6-4"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2//6-4"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("()"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("((()))"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("1+()"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("1-()"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2*()"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("2/()"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("4+4*()+8"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(4+5)*12 - (4*()+8)"), Catch::Contains(invalidTokenMessage) );

        CHECK_THROWS_WITH( ParseAndCompute("(123)4"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(22+4)55"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123-4(66)"), Catch::Contains(invalidTokenMessage) );
    }

    const char* rightBraceIsNotFoundMessage = "Error! Right brace is not found!";
    SECTION( "right brace is not found" ) {
        CHECK_THROWS_WITH( ParseAndCompute("(5"), Catch::Contains(rightBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(80+4"), Catch::Contains(rightBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(80-4"), Catch::Contains(rightBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(80*4"), Catch::Contains(rightBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(80/4"), Catch::Contains(rightBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("(80-(5 + 6)*4"), Catch::Contains(rightBraceIsNotFoundMessage) );
    }

    const char* leftBraceIsNotFoundMessage = "Error! Left brace is not found for right brace!";
    SECTION( "left brace is not found" ) {
        CHECK_THROWS_WITH( ParseAndCompute(")"), Catch::Contains(invalidTokenMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123)"), Catch::Contains(leftBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123+4)"), Catch::Contains(leftBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123-4)"), Catch::Contains(leftBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123*4)"), Catch::Contains(leftBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("123/4)"), Catch::Contains(leftBraceIsNotFoundMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("4+5+8*4-123/4) / 2 * 3"), Catch::Contains(leftBraceIsNotFoundMessage) );
    }

    const char* emptyExpressionMessage = "Error! Expression is empty or contains only spaced or tabs!";
    SECTION( "empty expressions" ) {
        CHECK_THROWS_WITH( ParseAndCompute(""), Catch::Contains(emptyExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute(" "), Catch::Contains(emptyExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("        "), Catch::Contains(emptyExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("        "), Catch::Contains(emptyExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("\t"), Catch::Contains(emptyExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("\t\t\t\t\t\t"), Catch::Contains(emptyExpressionMessage) );
        CHECK_THROWS_WITH( ParseAndCompute("\t   \t     "), Catch::Contains(emptyExpressionMessage) );
    }
}



