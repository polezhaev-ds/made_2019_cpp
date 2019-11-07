//
// Created by admin2 on 05.11.2019.
//

#define CATCH_CONFIG_MAIN

#include <string>
#include <vector>
#include <iostream>
#include "catch2/catch.hpp"
#include "Tokenizer.h"
#include "TokenInfo.h"

enum CallbackType: int {
    OnParseStarted = 1,
    OnParseFinished = 2,
    OnNextToken = 4,
    OnNextNumericToken = 8,
    OnNextStringToken = 16
};


const int EMPTY_CALLBACK_TYPES = 0;
const int ALL_CALLBACK_TYPES = CallbackType::OnParseStarted |
        CallbackType::OnParseFinished |
        CallbackType::OnNextToken |
        CallbackType::OnNextNumericToken |
        CallbackType::OnNextStringToken;

inline bool isTypeIn(CallbackType type, int flagBits) {
    return type & flagBits;
}

void getTokenizerResultsUsingCallbacks(const std::string& strToTokenize, bool& parseStarted, bool& parseFinished,
                                       std::vector<TokenInfo>& allTokens, std::vector<TokenInfo>& numericTokens,
                                       std::vector<TokenInfo>& stringTokens, int setCallbackTypeFlags,
                                       int unsetCallbackTypeFlags) {
    parseStarted = parseFinished = false;
    allTokens.clear();
    numericTokens.clear();
    stringTokens.clear();

    Tokenizer tokenizer;
    if (isTypeIn(CallbackType::OnParseStarted, setCallbackTypeFlags))
        tokenizer.setOnParseStartedHandler([&parseStarted, &strToTokenize](const std::string& str) {
            parseStarted = str == strToTokenize;
        });
    if (isTypeIn(CallbackType::OnParseFinished, setCallbackTypeFlags))
        tokenizer.setOnParseFinishedHandler([&parseFinished, &strToTokenize](const std::string& str) {
            parseFinished = str == strToTokenize;
        });
    if (isTypeIn(CallbackType::OnNextToken, setCallbackTypeFlags))
        tokenizer.setOnNextTokenHandler([&allTokens](const std::string& nextToken, int position) {
            allTokens.push_back(TokenInfo(nextToken, position));
        });
    if (isTypeIn(CallbackType::OnNextNumericToken, setCallbackTypeFlags))
        tokenizer.setOnNextNumericTokenHandler([&numericTokens](const std::string& nextToken, int position) {
            numericTokens.push_back(TokenInfo(nextToken, position));
        });
    if (isTypeIn(CallbackType::OnNextStringToken, setCallbackTypeFlags))
        tokenizer.setOnNextStringTokenHandler([&stringTokens](const std::string& nextToken, int position) {
            stringTokens.push_back(TokenInfo(nextToken, position));
        });

    if (isTypeIn(CallbackType::OnParseStarted, unsetCallbackTypeFlags))
        tokenizer.unsetOnParseStartedHandler();
    if (isTypeIn(CallbackType::OnParseFinished, unsetCallbackTypeFlags))
        tokenizer.unsetOnParseFinishedHandler();
    if (isTypeIn(CallbackType::OnNextToken, unsetCallbackTypeFlags))
        tokenizer.unsetOnNextTokenHandler();
    if (isTypeIn(CallbackType::OnNextNumericToken, unsetCallbackTypeFlags))
        tokenizer.unsetOnNextNumericTokenHandler();
    if (isTypeIn(CallbackType::OnNextStringToken, unsetCallbackTypeFlags))
        tokenizer.unsetOnNextStringTokenHandler();
    tokenizer.parse(strToTokenize);
}

void checkTokenizerResults(const std::string& strToTokenize, const std::vector<TokenInfo>& allExpectedTokens,
                           const std::vector<TokenInfo>& expectedNumericTokens, const std::vector<TokenInfo>& expectedStringTokens,
                           int setCallbackTypeFlags = ALL_CALLBACK_TYPES, int unsetCallbackTypeFlags = EMPTY_CALLBACK_TYPES) {
    bool parseStarted, parseFinished;
    std::vector<TokenInfo> allTokens, numericTokens, stringTokens, invalidTokens;

    getTokenizerResultsUsingCallbacks(strToTokenize, parseStarted, parseFinished, allTokens, numericTokens,
        stringTokens, setCallbackTypeFlags, unsetCallbackTypeFlags);

    CHECK( (parseStarted && isTypeIn(CallbackType::OnParseStarted, setCallbackTypeFlags) &&
             !isTypeIn(CallbackType::OnParseStarted, unsetCallbackTypeFlags) ||
           !parseStarted && (!isTypeIn(CallbackType::OnParseStarted, setCallbackTypeFlags) ||
             isTypeIn(CallbackType::OnParseStarted, unsetCallbackTypeFlags))) );
    CHECK( (parseFinished && isTypeIn(CallbackType::OnParseFinished, setCallbackTypeFlags) &&
             !isTypeIn(CallbackType::OnParseFinished, unsetCallbackTypeFlags) ||
           !parseFinished && (!isTypeIn(CallbackType::OnParseFinished, setCallbackTypeFlags) ||
             isTypeIn(CallbackType::OnParseFinished, unsetCallbackTypeFlags))) );
    CHECK( allTokens == allExpectedTokens );
    CHECK( numericTokens == expectedNumericTokens );
    CHECK( stringTokens == expectedStringTokens );
}

void checkEmptyCase(const std::string& strToTokenize) {
    checkTokenizerResults(strToTokenize, {}, {}, {});
}

void checkSingleNumericTokenCase(const std::string& strToTokenize, const TokenInfo& token) {
    checkTokenizerResults(strToTokenize, {token}, {token}, {});
}

void checkSingleStringTokenCase(const std::string& strToTokenize, const TokenInfo& token) {
    checkTokenizerResults(strToTokenize, {token}, {}, {token});
}

void checkWhenOnlyNumericTokensCase(const std::string& strToTokenize, const std::vector<TokenInfo>& tokens) {
    checkTokenizerResults(strToTokenize, tokens, tokens, {});
}

void checkWhenOnlyStringTokensCase(const std::string& strToTokenize, const std::vector<TokenInfo>& tokens) {
    checkTokenizerResults(strToTokenize, tokens, {}, tokens);
}


TEST_CASE( "tests for strings without tokens" ) {

    SECTION( "Tokenizer works correctly for empty string" ) {
        checkEmptyCase("");
    }

    SECTION( "Tokenizer works correctly for empty string with spaces" ) {
        checkEmptyCase("     ");
    }

    SECTION( "Tokenizer works correctly for empty string with tabs" ) {
        checkEmptyCase("\t\t\t\t");
    }

    SECTION( "Tokenizer works correctly for empty string with new lines" ) {
        checkEmptyCase("\n\n\n\n");
    }

    SECTION( "Tokenizer works correctly for empty string with spaces, tabs and new lines" ) {
        checkEmptyCase("  \t \n  \t  \t\t \n ");
    }

}

TEST_CASE( "tests for a single token" ) {

    SECTION( "Tokenizer works correctly for a single numeric token" ) {
        checkSingleNumericTokenCase("1", {"1", 0});
        checkSingleNumericTokenCase("33", {"33", 0});
        checkSingleNumericTokenCase("222", {"222", 0});
        checkSingleNumericTokenCase("  222  ", {"222", 2});
        checkSingleNumericTokenCase("\n\t222  ", {"222", 2});
    }

    SECTION( "Tokenizer works correctly for a single string token" ) {
        checkSingleStringTokenCase("a", {"a", 0});
        checkSingleStringTokenCase("#", {"#", 0});
        checkSingleStringTokenCase("abc", {"abc", 0});
        checkSingleStringTokenCase("1q", {"1q", 0});
        checkSingleStringTokenCase("q1", {"q1", 0});
        const char* latinAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        checkSingleStringTokenCase(latinAlphabet, {latinAlphabet, 0});
        const char* specialCharacters = "~!@#$%^&*()_+`{}[];'<>,.?/\\|";
        checkSingleStringTokenCase(specialCharacters, {specialCharacters, 0});
        checkSingleStringTokenCase(" \n \t abc@#$213@#$@#$  ", {"abc@#$213@#$@#$", 5});
    }

}

TEST_CASE( "tests for a single type tokens" ) {

    SECTION( "Tokenizer works correctly when only numeric tokens" ) {
        checkWhenOnlyNumericTokensCase("112 222", {{"112", 0}, {"222", 4}});
        checkWhenOnlyNumericTokensCase("  \n123\n 223\n1111 \t12",
                {{"123", 3}, {"223", 8}, {"1111", 12},
                {"12", 18}});
    }

    SECTION( "Tokenizer works correctly when only string tokens" ) {
        checkWhenOnlyStringTokensCase("abc def", {{"abc", 0}, {"def", 4}});
        checkWhenOnlyStringTokensCase("  \nabc\n ttT\n#$@! \tW#",
        {{"abc", 3}, {"ttT", 8}, {"#$@!", 12},
                {"W#", 18}});
        checkWhenOnlyStringTokensCase("a1c d2f", {{"a1c", 0}, {"d2f", 4}});
        checkWhenOnlyStringTokensCase("  \n1bc\n 1_3\n#$2! \tW2",
            {{"1bc", 3}, {"1_3", 8}, {"#$2!", 12}, {"W2", 18}});
    }

}


TEST_CASE( "tests for mixed type tokens" ) {
    checkTokenizerResults("\n\thello WORLD! 34234",
        // All expected tokens
        {{"hello", 2}, {"WORLD!", 8}, {"34234", 15}},
        // Expected numeric tokens
        {{"34234", 15}},
        // Expected string tokens
        {{"hello", 2}, {"WORLD!", 8}});
    checkTokenizerResults("\n\td3423d\n 123123 34234 aaa!!!",
        // All expected tokens
        {{"d3423d", 2}, {"123123", 10}, {"34234", 17},
            {"aaa!!!", 23}},
        // Expected numeric tokens
        {{"123123", 10}, {"34234", 17}},
        // Expected string tokens
        {{"d3423d", 2}, {"aaa!!!", 23}});
    checkTokenizerResults(" assdfsdf  123123121231232321  sdfsdf  ABFSDFSD  12345645645 dsadas123123 @#$@#%$@#$@#$",
        // All expected tokens
        {{"assdfsdf", 1}, {"123123121231232321", 11},
            {"sdfsdf", 31}, {"ABFSDFSD", 39}, {"12345645645", 49},
            {"dsadas123123", 61}, {"@#$@#%$@#$@#$", 74}},
        // Expected numeric tokens
        {{"123123121231232321", 11}, {"12345645645", 49}},
        // Expected string tokens
        {{"assdfsdf", 1}, {"sdfsdf", 31}, {"ABFSDFSD", 39}, {"dsadas123123", 61}, {"@#$@#%$@#$@#$", 74}, });
}

TEST_CASE( "check correctness of set and unset handler methods of Tokenizer" ) {

    SECTION( "set handler methods of Tokenizer return current instance" ) {
        Tokenizer tokenizer;

        CHECK( &tokenizer.setOnParseStartedHandler([](const std::string& str) {}) == &tokenizer );
        CHECK( &tokenizer.setOnParseFinishedHandler([](const std::string& str) {}) == &tokenizer );
        CHECK( &tokenizer.setOnNextTokenHandler([](const std::string& nextToken, int position ) {}) == &tokenizer );
        CHECK( &tokenizer.setOnNextNumericTokenHandler([](const std::string& nextToken, int position ) {}) == &tokenizer );
        CHECK( &tokenizer.setOnNextStringTokenHandler([](const std::string& nextToken, int position ) {}) == &tokenizer );
    }

    SECTION( "unset handler methods of Tokenizer return current instance" ) {
        Tokenizer tokenizer;

        CHECK( &tokenizer.unsetOnParseStartedHandler() == &tokenizer );
        CHECK( &tokenizer.unsetOnParseFinishedHandler() == &tokenizer );
        CHECK( &tokenizer.unsetOnNextTokenHandler() == &tokenizer );
        CHECK( &tokenizer.unsetOnNextNumericTokenHandler() == &tokenizer );
        CHECK( &tokenizer.unsetOnNextStringTokenHandler() == &tokenizer );
    }

    SECTION( "check that set methods of Tokenizer set callbacks" ) {
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {},
            // Expected numeric tokens
            {},
            // Expected string tokens
            {},
            // Set only one callback
            CallbackType::OnParseStarted,
            // Don't unset callbacks
            EMPTY_CALLBACK_TYPES);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {},
            // Expected numeric tokens
            {},
            // Expected string tokens
            {},
            // Set only one callback
            CallbackType::OnParseFinished,
            // Don't unset callbacks
            EMPTY_CALLBACK_TYPES);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {{"abc", 0}, {"123", 4}, {"ab1", 8}},
            // Expected numeric tokens
            {},
            // Expected string tokens
            {},
            // Set only one callback
            CallbackType::OnNextToken,
            // Don't unset callbacks
            EMPTY_CALLBACK_TYPES);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {},
            // Expected numeric tokens
            {{"123", 4}},
            // Expected string tokens
            {},
            // Set only one callback
            CallbackType::OnNextNumericToken,
            // Don't unset callbacks
            EMPTY_CALLBACK_TYPES);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {},
            // Expected numeric tokens
            {},
            // Expected string tokens
            {{"abc", 0}, {"ab1", 8}},
            // Set only one callback
            CallbackType::OnNextStringToken,
            // Don't unset callbacks
            EMPTY_CALLBACK_TYPES);
    }

    SECTION( "check that unset methods of Tokenizer remove callbacks" ) {
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {{"abc", 0}, {"123", 4}, {"ab1", 8}},
            // Expected numeric tokens
            {{"123", 4}},
            // Expected string tokens
            {{"abc", 0}, {"ab1", 8}},
            // Set all callbacks
            ALL_CALLBACK_TYPES,
            // Unset only one callback
            CallbackType::OnParseStarted);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {{"abc", 0}, {"123", 4}, {"ab1", 8}},
            // Expected numeric tokens
            {{"123", 4}},
            // Expected string tokens
            {{"abc", 0}, {"ab1", 8}},
            // Set all callbacks
            ALL_CALLBACK_TYPES,
            // Unset only one callback
            CallbackType::OnParseFinished);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {},
            // Expected numeric tokens
            {{"123", 4}},
            // Expected string tokens
            {{"abc", 0}, {"ab1", 8}},
            // Set all callbacks
            ALL_CALLBACK_TYPES,
            // Unset only one callback
            CallbackType::OnNextToken);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {{"abc", 0}, {"123", 4}, {"ab1", 8}},
            // Expected numeric tokens
            {},
            // Expected string tokens
            {{"abc", 0}, {"ab1", 8}},
            // Set all callbacks
            ALL_CALLBACK_TYPES,
            // Unset only one callback
            CallbackType::OnNextNumericToken);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {{"abc", 0}, {"123", 4}, {"ab1", 8}},
            // Expected numeric tokens
            {{"123", 4}},
            // Expected string tokens
            {},
            // Set all callbacks
            ALL_CALLBACK_TYPES,
            /// Unset only one callback
            CallbackType::OnNextStringToken);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {},
            // Expected numeric tokens
            {},
            // Expected string tokens
            {},
            // Set all callbacks
            ALL_CALLBACK_TYPES,
            // All callbacks are unset
            ALL_CALLBACK_TYPES);
        checkTokenizerResults("abc 123 ab1",
            // All expected tokens
            {},
            // Expected numeric tokens
            {},
            // Expected string tokens
            {},
            // Don't set any callback
            EMPTY_CALLBACK_TYPES,
            // All callbacks are unset
            ALL_CALLBACK_TYPES);
    }
}


