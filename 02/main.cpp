#include <iostream>
#include <string>
#include "argparse.h"
#include "benchmark.h"

const size_t DEFAULT_MIN_RANDOM_REQUESTED_SIZE = 1024 * 1024;
const size_t DEFAULT_MAX_RANDOM_REQUESTED_SIZE = 1024 * 1024;
const int DEFAULT_NUMBER_OF_REQUESTS_PER_TEST = 800;
const int DEFAULT_NUMBER_OF_TESTS = 20;
const int DEFAULT_NUMBER_OF_WARMING_TESTS = 3;

void printHelp(char* const programName, ArgumentParser& parser) {
    parser.print_help();
    std::cout << "Usage example:" << std::endl;
    std::cout << "    " << programName
              << " -m " << DEFAULT_MIN_RANDOM_REQUESTED_SIZE
              << " -M " << DEFAULT_MIN_RANDOM_REQUESTED_SIZE
              << " -r " << DEFAULT_NUMBER_OF_REQUESTS_PER_TEST
              << " -t " << DEFAULT_NUMBER_OF_TESTS
              << " -w " << DEFAULT_NUMBER_OF_WARMING_TESTS
              << std::endl;
}

int main(int argc, char* argv[]) {

    size_t minRandomRequestedSize = DEFAULT_MIN_RANDOM_REQUESTED_SIZE;
    size_t maxRandomRequestedSize = DEFAULT_MAX_RANDOM_REQUESTED_SIZE;
    int numberOfRequestsPerTest = DEFAULT_NUMBER_OF_REQUESTS_PER_TEST;
    int numberOfTests = DEFAULT_NUMBER_OF_TESTS;
    int numberOfWarmingTests = DEFAULT_NUMBER_OF_WARMING_TESTS;

    ArgumentParser parser("Program to benchmark LinearAllocator va malloc");
    parser.add_argument("-m", "--min_size", "minimum random requested size (default " +
                                            std::to_string(DEFAULT_MIN_RANDOM_REQUESTED_SIZE) +  ")", false);
    parser.add_argument("-M", "--max_size", "maximum random requested size (default " +
                                            std::to_string(DEFAULT_MAX_RANDOM_REQUESTED_SIZE) +  ")", false);
    parser.add_argument("-r", "--requests", "number of allocation requests per test (default " +
                                            std::to_string(DEFAULT_NUMBER_OF_REQUESTS_PER_TEST) +  ")", false);
    parser.add_argument("-t", "--tests", "number of tests (default " +
                                         std::to_string(DEFAULT_NUMBER_OF_TESTS) +  ")", false);
    parser.add_argument("-w", "--warming", "number of warming tests (default " +
                                         std::to_string(DEFAULT_NUMBER_OF_WARMING_TESTS) +  ")", false);
    try {
        parser.parse(argc, argv);
    } catch (const ArgumentParser::ArgumentNotFound& ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    if (parser.is_help()) {
        printHelp(argv[0], parser);
        return 0;
    }

    try {
        if (parser.exists("m"))
            minRandomRequestedSize = std::stoull(parser.get<std::string>("m"));
        if (parser.exists("M"))
            maxRandomRequestedSize = std::stoull(parser.get<std::string>("M"));
        if (parser.exists("r"))
            numberOfRequestsPerTest = std::stoi(parser.get<std::string>("r"));
        if (parser.exists("t"))
            numberOfTests = std::stoi(parser.get<std::string>("t"));
        if (parser.exists("w"))
            numberOfWarmingTests = std::stoi(parser.get<std::string>("w"));
    } catch (const std::invalid_argument& ex) {
        std::cout << "Error! Invalid input arguments!" << std::endl;
        printHelp(argv[0], parser);
        return 2;
    } catch (const std::out_of_range& ex) {
        std::cout << "Error! Input argument is out of range!" << std::endl;
        return 3;
    }

    std::cout << "Using the following benchmark parameters:" << std::endl;
    std::cout << " minimum random requested size (--min_size or -m): " << minRandomRequestedSize << std::endl;
    std::cout << " maximum random requested size (--max_size or -M): " << maxRandomRequestedSize << std::endl;
    std::cout << " number of allocation requests per test (--requests or -r): " << numberOfRequestsPerTest << std::endl;
    std::cout << " number of tests (--tests or -t): " << numberOfTests << std::endl;
    std::cout << " number of warming tests (--warming or -w): " << numberOfWarmingTests << std::endl;

    std::tuple<double, double> averageTimes = benchmarkAllocators(
            minRandomRequestedSize, maxRandomRequestedSize,
            numberOfRequestsPerTest, numberOfTests, numberOfWarmingTests);

    std::cout.setf(std::ios::fixed | std::ios::showpoint);
    std::cout.precision(3);
    std::cout << "Average execution time for:" << std::endl;
    std::cout << "  LinearAllocator: " << std::get<0>(averageTimes) << " microseconds" << std::endl;
    std::cout << "           malloc: " << std::get<1>(averageTimes) << " microseconds" << std::endl;

    return 0;
}


