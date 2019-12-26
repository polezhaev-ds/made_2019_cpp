//
// Created by admin2 on 17.11.2019.
//

#define CATCH_CONFIG_MAIN

#include <stdexcept>
#include <random>
#include <numeric>
#include <vector>
#include <iterator>
#include "catch2/catch.hpp"
#include "ThreadPool.h"

template <class Iterator, class T = typename Iterator::value_type, typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
void fill_vector(Iterator first, Iterator last) {
    std::mt19937 engine;
    std::uniform_int_distribution<T> distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    auto generatorLambda = [&engine, &distribution]() mutable {
        return distribution(engine);
    };
    std::generate(first, last, generatorLambda);
}

template <class Iterator, class T>
T parallelAccumulate(Iterator first, Iterator last, T init, std::size_t poolSize, std::size_t blockSize = 50) {
    std::ptrdiff_t diff = std::distance(first, last);
    if (diff <= 0)
        return 0;
    std::size_t numberOfBlocks = (diff + blockSize - 1) / blockSize;

    ThreadPool threadPool(poolSize);

    std::vector<std::future<T>> futures;
    futures.reserve(numberOfBlocks);

    Iterator blockStart = first;
    Iterator blockEnd;
    for (std::size_t i = 0; i < numberOfBlocks; ++i) {
        if (i < numberOfBlocks - 1) {
            blockEnd = blockStart;
            std::advance(blockEnd, blockSize);
        } else {
            blockEnd = last;
        }
        futures.emplace_back(threadPool.exec([](Iterator block_start, Iterator block_end){
            return std::accumulate(block_start, block_end, T());
        }, blockStart, blockEnd));
        blockStart = blockEnd;
    }

    T result = init;
    for (std::size_t i = 0; i < numberOfBlocks; ++i)
        result += futures[i].get();
    return result;
}

template <class T>
void testThreadPoolForParallelAccumulate(std::size_t poolSize) {
    const std::size_t size = 10'000'000ull;
    std::vector<T> vec(size);
    fill_vector(std::begin(vec), std::end(vec));
    CHECK ( parallelAccumulate(std::begin(vec), std::end(vec), 0, poolSize, 20) == std::accumulate(std::begin(vec), std::end(vec), 0) );
    CHECK ( parallelAccumulate(std::begin(vec), std::end(vec), 0, poolSize, 2000) == std::accumulate(std::begin(vec), std::end(vec), 0) );
}


TEST_CASE( "Test class ThreadPool" ) {

    SECTION( "Test when number of threads is zero" ) {
        std::size_t cores_count = std::thread::hardware_concurrency();
        if (cores_count == 0)
            cores_count = 1;
        CHECK( ThreadPool(0).GetPoolSize() == cores_count );
    }

    SECTION( "Test when number of threads is one" ) {
        testThreadPoolForParallelAccumulate<int>(1);
        testThreadPoolForParallelAccumulate<std::size_t>(1);
    }

    SECTION( "Test when number of threads is two" ) {
        testThreadPoolForParallelAccumulate<int>(2);
        testThreadPoolForParallelAccumulate<std::size_t>(2);
    }

    SECTION( "Test when number of threads is 4" ) {
        testThreadPoolForParallelAccumulate<int>(4);
        testThreadPoolForParallelAccumulate<std::size_t>(4);
    }

    SECTION( "Test when number of threads is 20" ) {
        testThreadPoolForParallelAccumulate<int>(20);
        testThreadPoolForParallelAccumulate<std::size_t>(20);
    }

    SECTION( "Test when executed lambda function returns void" ) {
        using namespace std::chrono_literals;
        std::size_t size = 8;
        ThreadPool pool(size);
        auto future = pool.exec([](){
            std::this_thread::sleep_for(3s);
        });
        CHECK( (std::is_same<decltype(future), std::future<void>>::value) );
        CHECK_NOTHROW( future.get() );
    }

    SECTION( "Test when functions with different signature are executed" ) {
        using namespace std::chrono_literals;
        std::size_t size = 2;
        ThreadPool pool(size);
        auto future_void = pool.exec([](){
            std::this_thread::sleep_for(3s);
        });
        std::string helloWorld = "Hello world!";
        auto future_string = pool.exec([](std::string& s){
            std::this_thread::sleep_for(3s);
            return s;
        }, helloWorld);
        auto future_int = pool.exec([](int a, int b){
            std::this_thread::sleep_for(3s);
            return a + b;
        }, 1000, 2000);

        CHECK( (std::is_same<decltype(future_void), std::future<void>>::value) );
        CHECK( (std::is_same<decltype(future_string), std::future<std::string>>::value) );
        CHECK( (std::is_same<decltype(future_int), std::future<int>>::value) );
        CHECK_NOTHROW( future_void.get() );
        CHECK( future_string.get() == helloWorld );
        CHECK( future_int.get() == 3000 );
    }


}

