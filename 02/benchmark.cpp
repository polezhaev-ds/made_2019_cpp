//
// Created by admin2 on 06.11.2019.
//

#include <iostream>
#include <chrono>
#include <memory>
#include <cstring>
#include <random>
#include "benchmark.h"
#include "LinearAllocator.h"


void printProgressBar(double progressRate, int barWidth) {
    std::cout << "\r[";
    int position = progressRate * barWidth;
    for (int i = 0; i < barWidth; i++)
        if (i < position)
            std::cout << "=";
        else if (i == position)
            std::cout << ">";
        else
            std::cout << " ";
    std::cout << "] " << int(progressRate * 100) << " %" << std::flush;
}

int testLinearAllocator(int numberOfRequestsPerTest, size_t allocatedSize,
                        const size_t* requestedSizes, char** pointers)
{
    LinearAllocator allocator(allocatedSize);

    int dummy_sum = 0; //To prevent optimization
    for (int j = 0; j < numberOfRequestsPerTest; j++) {
        asm(""); //To prevent optimization
        pointers[j] = allocator.allocate(requestedSizes[j]);
        std::memset(const_cast<char *>(pointers[j]), j % 256, requestedSizes[j]);
        dummy_sum += *pointers[j];
    }
    return dummy_sum; //To prevent optimization
}

int testMallocAllocator(int numberOfRequestsPerTest, size_t allocatedSize,
                        const size_t* requestedSizes, char** pointers)
{
    int dummy_sum = 0; //To prevent optimization
    for (int j = 0; j < numberOfRequestsPerTest; j++) {
        asm(""); //To prevent optimization
        pointers[j] = (char *) std::malloc(requestedSizes[j]);
        std::memset(const_cast<char *>(pointers[j]), j % 256, requestedSizes[j]);
        dummy_sum += *pointers[j];
    }
    for (int j = 0; j < numberOfRequestsPerTest; j++) {
        asm(""); //To prevent optimization
        std::free(const_cast<char *>(pointers[j]));
    }
    return dummy_sum; //To prevent optimization
}

double runSingleTest(const TestFunction& benchmarkedFunction, int numberOfRequestsPerTest, size_t allocatedSize,
                     const size_t* requestedSizes, char** pointers)
{
    auto linearAllocatorBeginTime = std::chrono::high_resolution_clock::now();
    benchmarkedFunction(numberOfRequestsPerTest, allocatedSize, requestedSizes, pointers);
    auto linearAllocatorEndTime = std::chrono::high_resolution_clock::now();
    return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(
            linearAllocatorEndTime - linearAllocatorBeginTime).count());
}

double runAllTests(const TestFunction& benchmarkedFunction, int numberOfRequestsPerTest, int numberOfTests,
                   int numberOfWarmingTests, size_t allocatedSize, const size_t* requestedSizes, char** pointers)
{
    int totalNumberOfTests = numberOfWarmingTests + numberOfTests;

    double totalDuration = 0;
    for (int i = 0; i < totalNumberOfTests; i++) {
        printProgressBar(static_cast<double>(i) / totalNumberOfTests);
        double duration = runSingleTest(testLinearAllocator, numberOfRequestsPerTest, allocatedSize, requestedSizes,
                                        pointers);
        if (i >= numberOfWarmingTests)
            totalDuration += duration;
    }
    printProgressBar(1.0);
    return totalDuration / numberOfTests;
}

std::tuple<double, double> benchmarkAllocators(size_t minRandomRequestedSize, size_t maxRandomRequestedSize,
                                               int numberOfRequestsPerTest, int numberOfTests, int numberOfWarmingTests) {
    std::random_device randomDevice;
    std::default_random_engine defaultRandomEngine( randomDevice() );
    std::uniform_int_distribution<size_t> uniformDistribution(minRandomRequestedSize, maxRandomRequestedSize);

    size_t* requestedSizes = new size_t[numberOfRequestsPerTest];
    char** pointers = new char* [numberOfRequestsPerTest];

    size_t allocatedSize = 0;
    std::cout << "Generating random sizes for allocation requests... ";
    for (int i = 0; i < numberOfRequestsPerTest; i++) {
        requestedSizes[i] = uniformDistribution(defaultRandomEngine);
        allocatedSize += requestedSizes[i];
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting to benchmark LinearAllocator..." << std::endl;
    double averageLinearAllocatorDuration = runAllTests(testLinearAllocator, numberOfRequestsPerTest, numberOfTests,
                                                        numberOfWarmingTests, allocatedSize, requestedSizes, pointers);
    std::cout << " Done"<< std::endl;

    std::cout << "Starting to benchmark malloc..." << std::endl;
    double averageMallocAllocatorDuration = runAllTests(testMallocAllocator, numberOfRequestsPerTest, numberOfTests,
                                                        numberOfWarmingTests, allocatedSize, requestedSizes, pointers);
    std::cout << " Done"<< std::endl;

    delete[] pointers;
    delete[] requestedSizes;

    return std::make_tuple(averageLinearAllocatorDuration, averageMallocAllocatorDuration);
}