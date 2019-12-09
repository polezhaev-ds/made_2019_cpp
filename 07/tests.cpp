//
// Created by admin2 on 17.11.2019.
//

#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "format.h"

TEST_CASE( "Test format function for correct formatting" ) {

    SECTION( "Test format function for the cases when format string is empty, has whitespaces or has no arguments " ) {
        CHECK( Format("") == "" );
        CHECK( Format(" ") == " " );
        CHECK( Format("\t") == "\t" );
        CHECK( Format("\n") == "\n" );
        CHECK( Format(" \n   \t   \t    ") == " \n   \t   \t    " );
        CHECK( Format("1") == "1" );
        CHECK( Format("23423454612423423") == "23423454612423423" );
        CHECK( Format("This string has no format arguments") == "This string has no format arguments" );
        CHECK( Format("33 cows grazed in the rich green meadow!") == "33 cows grazed in the rich green meadow!" );
    }

    SECTION( "Test format function for the cases when format string has a single argument" ) {
        CHECK( Format("{0}", "") == "" );
        CHECK( Format(" {0} ", "") == "  " );
        CHECK( Format("{0}", true) == "1" );
        CHECK( Format("{0}", '@') == "@" );
        CHECK( Format("{0}", static_cast<signed char>(97)) == "a" );
        CHECK( Format("{0}", static_cast<unsigned char>(97)) == "a" );
        CHECK( Format("{0}", static_cast<short int>(-12395)) == "-12395" );
        CHECK( Format("{0}", static_cast<unsigned short int>(62395)) == "62395" );
        CHECK( Format("{0}", 777) == "777" );
        CHECK( Format("{0}", -777) == "-777" );
        CHECK( Format("{0}", 1234567890ul) == "1234567890" );
        CHECK( Format("{0}", -1234567890123123ll) == "-1234567890123123" );
        CHECK( Format("{0}", 1234567890123123ull) == "1234567890123123" );
        CHECK( Format("{0}", -1.2345) == "-1.2345" );
        CHECK( Format("{0}", "Versicolor") == "Versicolor" );
        CHECK( Format("Iris {0} is one of the amazing flowers!", "Versicolor") == "Iris Versicolor is one of the amazing flowers!" );
        CHECK( Format("{0}{0}", "Stroustrup") == "StroustrupStroustrup" );
        CHECK( Format(" {0} Bjarne {0} ", "Stroustrup") == " Stroustrup Bjarne Stroustrup " );
    }

    SECTION( "Test format function for the cases when format string has two or more arguments" ) {
        CHECK( Format("{0}", 777) == "777" );
        CHECK( Format("{1} {2} {0}", "Standard", "Template", "Library") == "Template Library Standard" );
        CHECK( Format("{2}{2} cows and a glass of {0}{1}", "milk", '!', 3) == "33 cows and a glass of milk!" );
        CHECK( Format("{0}{1}{2}{3}{4}{5}", 1, true, '!', 22ul, "qwe", 2.3) == "11!22qwe2.3" );
        CHECK( Format("{0}{1}{2}{3}{4}{5}{6}{7}{8}{9}{10}{11}", 0, 111, 222, 333, 444, "abc", '@', 777, 88, 9, 10, 11) ==
            "0111222333444abc@7778891011" );
    }
}

TEST_CASE( "Test format function for incorrect formatting" ) {

    SECTION( "Tests when opening brace is missed" ) {
        REQUIRE_THROWS_WITH( Format("0}", 5), Catch::Contains("Opening brace '{' was not found at position 1") );
        REQUIRE_THROWS_AS( Format("0}", 5), std::runtime_error );
        REQUIRE_THROWS_WITH( Format(" 0}", 5), Catch::Contains("Opening brace '{' was not found at position 2") );
        REQUIRE_THROWS_WITH( Format(" {1}0}", 55, 66), Catch::Contains("Opening brace '{' was not found at position 5") );
        REQUIRE_THROWS_WITH( Format("}", 5), Catch::Contains("Opening brace '{' was not found at position 0") );
        REQUIRE_THROWS_AS( Format("}", 5), std::runtime_error );
        REQUIRE_THROWS_WITH( Format(" }", 5), Catch::Contains("Opening brace '{' was not found at position 1") );
        REQUIRE_THROWS_WITH( Format(" }{0}", 55, 66), Catch::Contains("Opening brace '{' was not found at position 1") );
    }

    SECTION( "Tests when closing brace is missed" ) {
        REQUIRE_THROWS_WITH( Format("{0", 5), Catch::Contains("Closing brace '}' was not found at position 2") );
        REQUIRE_THROWS_AS( Format("{0", 5), std::runtime_error );
        REQUIRE_THROWS_WITH( Format(" {0", 5), Catch::Contains("Closing brace '}' was not found at position 3") );
        REQUIRE_THROWS_WITH( Format(" {1{0}", 55, 66), Catch::Contains("Closing brace '}' was not found at position 3") );
    }

    SECTION( "Tests when closing brace and argument index are missed" ) {
        REQUIRE_THROWS_WITH( Format("{", 5), Catch::Contains("Argument index and closing brace '}' was not found at position 1") );
        REQUIRE_THROWS_AS( Format("{", 5), std::runtime_error );
        REQUIRE_THROWS_WITH( Format(" {", 5), Catch::Contains("Argument index and closing brace '}' was not found at position 2") );
        REQUIRE_THROWS_WITH( Format(" {{0}", 55, 66), Catch::Contains("Argument index and closing brace '}' was not found at position 2") );
    }

    SECTION( "Tests when argument index ia missed" ) {
        REQUIRE_THROWS_WITH( Format("{}", 5), Catch::Contains("Argument index was not found at position 1") );
        REQUIRE_THROWS_AS( Format("{}", 5), std::runtime_error );
        REQUIRE_THROWS_WITH( Format(" {}", 5), Catch::Contains("Argument index was not found at position 2") );
        REQUIRE_THROWS_WITH( Format(" {}{1}", 55, 66), Catch::Contains("Argument index was not found at position 2") );
    }

    SECTION( "Tests for invalid arguments" ) {
        REQUIRE_THROWS_WITH( Format("{1}", 5), Catch::Contains("Invalid argument index 1") );
        REQUIRE_THROWS_AS( Format("{1}", 5), std::runtime_error );
        REQUIRE_THROWS_WITH( Format(" {2}", 5), Catch::Contains("Invalid argument index 2") );
        REQUIRE_THROWS_WITH( Format("{0}{3}", 55, 66), Catch::Contains("Invalid argument index 3") );
    }

    SECTION( "Tests for invalid symbols in arguments" ) {
        REQUIRE_THROWS_WITH( Format("{@1}", 5), Catch::Contains("Invalid symbol '@' was found at position 1") );
        REQUIRE_THROWS_AS( Format("{@1}", 5), std::runtime_error );
        REQUIRE_THROWS_WITH( Format("{1@}", 5), Catch::Contains("Invalid symbol '@' was found at position 2") );
        REQUIRE_THROWS_WITH( Format("{0}{1#}", 5, 6), Catch::Contains("Invalid symbol '#' was found at position 5") );
    }
}

