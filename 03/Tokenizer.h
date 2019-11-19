//
// Created by admin2 on 06.11.2019.
//

#ifndef HW3_TOKENIZER_H
#define HW3_TOKENIZER_H

#include <functional>
#include <string>
#include <cstddef>

using ParsingHandler = std::function<void (const std::string& str)>;
using NextTokenParsedHandler = std::function<void (const std::string& nextToken, std::size_t position)>;


class Tokenizer {
public:
    explicit Tokenizer():
        onParseStarted(nullptr),
        onParseFinished(nullptr),
        onNextToken(nullptr),
        onNextNumericToken(nullptr),
        onNextStringToken(nullptr)
    {
    }

    void parse(const std::string& str) const;

    Tokenizer& setOnParseStartedHandler(const ParsingHandler& handler);
    Tokenizer& setOnParseFinishedHandler(const ParsingHandler& handler);
    Tokenizer& setOnNextTokenHandler(const NextTokenParsedHandler& handler);
    Tokenizer& setOnNextNumericTokenHandler(const NextTokenParsedHandler& handler);
    Tokenizer& setOnNextStringTokenHandler(const NextTokenParsedHandler& handler);

    Tokenizer& unsetOnParseStartedHandler();
    Tokenizer& unsetOnParseFinishedHandler();
    Tokenizer& unsetOnNextTokenHandler();
    Tokenizer& unsetOnNextNumericTokenHandler();
    Tokenizer& unsetOnNextStringTokenHandler();

private:
    ParsingHandler onParseStarted;
    ParsingHandler onParseFinished;

    NextTokenParsedHandler onNextToken;
    NextTokenParsedHandler onNextNumericToken;
    NextTokenParsedHandler onNextStringToken;
};


#endif //HW3_TOKENIZER_H
