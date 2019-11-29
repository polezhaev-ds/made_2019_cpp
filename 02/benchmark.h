//
// Created by admin2 on 06.11.2019.
//

#ifndef HW2_BENCHMARK_H
#define HW2_BENCHMARK_H


#include <functional>
#include <tuple>

using TestFunction = std::function<void (int numberOfRequestsPerTest, size_t allocatedSize,
                                         const size_t* requestedSizes, char** pointers)>;

void printProgressBar(double progressRate, int barWidth=60);

int testLinearAllocator(int numberOfRequestsPerTest, size_t allocatedSize,
                        const size_t* requestedSizes, char** pointers);

int testMallocAllocator(int numberOfRequestsPerTest, size_t allocatedSize,
                        const size_t* requestedSizes, char** pointers);

double runSingleTest(const TestFunction& benchmarkedFunction, int numberOfRequestsPerTest, size_t allocatedSize,
                     const size_t* requestedSizes, char** pointers);

double runAllTests(const TestFunction& benchmarkedFunction, int numberOfRequestsPerTest, int numberOfTests,
                   int numberOfWarmingTests, size_t allocatedSize, const size_t* requestedSizes, char** pointers);

std::tuple<double, double> benchmarkAllocators(size_t minRandomRequestedSize, size_t maxRandomRequestedSize,
                                               int numberOfRequestsPerTest, int numberOfTests, int numberOfWarmingTests);


#endif //HW2_BENCHMARK_H
