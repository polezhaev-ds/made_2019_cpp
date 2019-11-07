#include <iostream>
#include <string>
#include "Tokenizer.h"

void printUsageExample() {
    std::cout << "  Examples:" << std::endl;
    std::cout << "    tokenizer \"5555 word 3333\"" << std::endl;
    std::cout << "    tokenizer \"123456789 @#$@#$@#% sdfs234234\"" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1)
    {
        std::cout << "Error! No arguments were passed! Please specify one argument with string to tokenize!" << std::endl;
        printUsageExample();
        return 1;

    } else if (argc > 2){
        std::cout << "Error! Too many arguments were passed! Please specify exactly one argument with string!" << std::endl;
        printUsageExample();
        return 2;
    }

    Tokenizer tokenizer;
    tokenizer.setOnParseStartedHandler([](const std::string &str) {
                std::cout << "Starting to tokenize string \"" << str << "\"" << std::endl;
            })
            .setOnParseFinishedHandler([](const std::string &str) {
                std::cout << "Finished to tokenize string \"" << str << "\"" << std::endl;
            })
            .setOnNextTokenHandler([](const std::string &nextToken, int position) {
                std::cout << "\tToken \"" << nextToken << "\" is found at position " << position << std::endl;
            })
            .setOnNextNumericTokenHandler([](const std::string &nextToken, int position) {
                std::cout << "\t\tNumeric  token \"" << nextToken << "\" is found at position " << position << std::endl;
            })
            .setOnNextStringTokenHandler([](const std::string &nextToken, int position) {
                std::cout << "\t\tString token \"" << nextToken << "\" is found at position " << position << std::endl;
            });
    tokenizer.parse(argv[1]);

    return 0;
}