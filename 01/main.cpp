#include <iostream>
#include "Parser.h"
#include "Exception.h"
#include "AnalyzerException.h"


void printUsageExample() {
    std::cout << "  Example:" << std::endl;
    std::cout << "    calc \"3+4*(2-4)\"" << std::endl;
    std::cout << "    calc \"123 / 4 * (2 + 4)\"" << std::endl;
}

void printBeautifiedError(const std::string& expression, int position) {
    std::cout << "  \"" << expression << "\"" << std::endl;
    for(int i = 0; i < position + 3; i++)
        std::cout << " ";
    std::cout << "^" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1)
    {
        std::cout << "Error! No arguments were passed! Please specify one argument with expression!" << std::endl;
        printUsageExample();
        return 1;

    } else if (argc > 2){
        std::cout << "Error! Too many arguments were passed! Please specify exactly one argument with expression!" << std::endl;
        printUsageExample();
        return 2;
    }
    std::string expression = argv[1];
    try {
        std::cout << "Expression: \"" << expression << "\"" << std::endl;
        int result = ParseAndCompute(expression);
        std::cout << "Result: " << result << std::endl;
    }
    catch (AnalyzerException& ex) {
        std::cout << ex.getMessage() << std::endl;
        printBeautifiedError(expression, ex.getPostion());
        return 3;
    }
    catch (Exception& ex) {
        std::cout << ex.getMessage() << std::endl;
        return 3;
    }
    return 0;
}