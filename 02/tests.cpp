//
// Created by admin2 on 05.11.2019.
//

#define CATCH_CONFIG_MAIN

#include <limits>
#include "catch2/catch.hpp"
#include "LinearAllocator.h"


inline void simpleTestsForSize(size_t maxSize) {
    LinearAllocator allocator(maxSize);
    REQUIRE( allocator.getMaxSize() == maxSize );
    REQUIRE( allocator.getAvailableSize() == maxSize );
    REQUIRE( allocator.allocate(0) == nullptr );
    char* pointer = allocator.allocate(1);
    *pointer = '#';
    REQUIRE( pointer != nullptr );
    REQUIRE( allocator.getAvailableSize() == maxSize - 1 );
    REQUIRE( allocator.getMaxSize() == maxSize );
    REQUIRE( allocator.allocate(2) == pointer + 1 );
    pointer[1] = '_';
    pointer[2] = '_';
    REQUIRE( allocator.getAvailableSize() == maxSize - 3);
    REQUIRE( allocator.allocate(maxSize - 3) == pointer + 3 );
    REQUIRE( allocator.getAvailableSize() == 0 );
    REQUIRE( allocator.getMaxSize() == maxSize );
    std::memset(pointer + 3, '@', maxSize - 3);
    REQUIRE( std::memcmp(pointer, "#__", 3) == 0 );
    char* expectedBytes = new char[maxSize - 3];
    std::memset(expectedBytes,'@', maxSize - 3);
    CHECK( std::memcmp(expectedBytes, pointer + 3, maxSize - 3) == 0 );
    delete[] expectedBytes;
    allocator.reset();
    REQUIRE( allocator.getMaxSize() == maxSize );
    REQUIRE( allocator.getAvailableSize() == maxSize );
    REQUIRE( allocator.allocate(0) == nullptr );
    REQUIRE( allocator.allocate(maxSize / 2) == pointer );
    std::memset(pointer, '@', maxSize / 2);
    REQUIRE( allocator.getAvailableSize() == maxSize - maxSize / 2 );
    REQUIRE( allocator.getMaxSize() == maxSize );
    REQUIRE( allocator.allocate(maxSize - maxSize / 2) == pointer + maxSize / 2 );
    std::memset(pointer + maxSize / 2, '_', maxSize - maxSize / 2);
    REQUIRE( allocator.getAvailableSize() == 0 );
    REQUIRE( allocator.getMaxSize() == maxSize );
    REQUIRE( allocator.allocate(1) == nullptr );
    REQUIRE( allocator.allocate(maxSize/2) == nullptr );
    REQUIRE( allocator.allocate(maxSize) == nullptr );
    REQUIRE( allocator.getAvailableSize() == 0 );
    expectedBytes = new char[maxSize];
    std::memset(expectedBytes,'@', maxSize / 2);
    std::memset(expectedBytes + maxSize / 2, '_', maxSize - maxSize / 2);
    CHECK(  std::memcmp(expectedBytes, pointer, maxSize) == 0  );
    delete[] expectedBytes;
    allocator.reset();
    REQUIRE( allocator.getMaxSize() == maxSize );
    REQUIRE( allocator.getAvailableSize() == maxSize );
    REQUIRE( allocator.allocate(maxSize) == pointer );
    std::memset(pointer, '%', maxSize);
    REQUIRE( allocator.getMaxSize() == maxSize );
    REQUIRE( allocator.getAvailableSize() == 0 );
    REQUIRE( allocator.allocate(maxSize) == nullptr );
    REQUIRE( allocator.getMaxSize() == maxSize );
    REQUIRE( allocator.getAvailableSize() == 0 );
    expectedBytes = new char [maxSize];
    std::memset(expectedBytes,'%', maxSize);
    CHECK( std::memcmp(expectedBytes, pointer, maxSize) == 0 );
    delete[] expectedBytes;
}

inline void loopTestsForSize(size_t maxSize, size_t numberOfIterations) {
    LinearAllocator allocator(maxSize);
    size_t sizePerIteration = maxSize / numberOfIterations;
    char *firstPointer = nullptr, *currentPointer = nullptr;
    for (size_t i = 0; i < numberOfIterations; i++) {
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize - i * sizePerIteration );
        size_t currentSize;
        if (i < numberOfIterations - 1)
            currentSize = sizePerIteration;
        else
            currentSize = maxSize - sizePerIteration * (numberOfIterations - 1);
        currentPointer = allocator.allocate(currentSize);
        if (firstPointer == nullptr)
            firstPointer = currentPointer;
        REQUIRE( currentPointer == firstPointer + i * sizePerIteration );

        std::memset(currentPointer, i % 256, currentSize);

    }
    REQUIRE( allocator.getMaxSize() == maxSize );
    REQUIRE( allocator.getAvailableSize() == 0 );
    for (size_t i = 0; i < numberOfIterations; i++) {
        size_t currentSize;
        if (i < numberOfIterations - 1)
            currentSize = sizePerIteration;
        else
            currentSize = maxSize - sizePerIteration * (numberOfIterations - 1);
        char* expectedBytes = new char [maxSize];
        std::memset(expectedBytes, i % 256, currentSize);
        CHECK( std::memcmp(expectedBytes, firstPointer + i * sizePerIteration, currentSize) == 0 );
        delete[] expectedBytes;
    }
}

TEST_CASE( "boundary cases" ) {

    SECTION( "LinearAllocator works properly for zero size of buffer" ) {
        LinearAllocator allocator(0);
        REQUIRE( allocator.getMaxSize() == 0 );
        REQUIRE( allocator.getAvailableSize() == 0 );
        REQUIRE( allocator.allocate(0) == nullptr );
        REQUIRE( allocator.allocate(1) == nullptr );
        REQUIRE( allocator.allocate(10) == nullptr );
        allocator.reset();
        REQUIRE( allocator.getMaxSize() == 0 );
        REQUIRE( allocator.getAvailableSize() == 0 );
        REQUIRE( allocator.allocate(0) == nullptr );
        REQUIRE( allocator.allocate(1) == nullptr );
        REQUIRE( allocator.allocate(10) == nullptr );
    }

    SECTION( "LinearAllocator works properly for very large size of buffer" ) {
        size_t maxValueOfSizeT = std::numeric_limits<size_t>::max();
        CHECK_THROWS_AS(LinearAllocator(maxValueOfSizeT), std::bad_alloc);
    }

}

TEST_CASE("other cases") {

    SECTION( "LinearAllocator works properly for a single allocation of maxSize size" ) {
        const size_t maxSize = 1024;
        LinearAllocator allocator(maxSize);
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
        char* pointer = allocator.allocate(maxSize);
        REQUIRE( pointer != nullptr );
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == 0 );
        allocator.reset();
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
    }

    SECTION( "LinearAllocator works properly for a single allocation of maxSize + 1 size" ) {
        const size_t maxSize = 1024;
        LinearAllocator allocator(maxSize);
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
        char* pointer = allocator.allocate(maxSize + 1);
        REQUIRE( pointer == nullptr );
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
        allocator.reset();
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
    }

    SECTION( "LinearAllocator works properly for a single allocation of maxSize + 1 size and consequent allocation" ) {
        const size_t maxSize = 1024;
        LinearAllocator allocator(maxSize);
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
        char* pointer = allocator.allocate(maxSize + 1);
        REQUIRE( pointer == nullptr );
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
        allocator.reset();
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
        pointer = allocator.allocate(maxSize);
        REQUIRE( pointer != nullptr );
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == 0 );
        allocator.reset();
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
    }

    SECTION( "LinearAllocator works properly, when reset is called first" ) {
        const size_t maxSize = 1024;
        LinearAllocator allocator(maxSize);
        allocator.reset();
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
        char* pointer = allocator.allocate(maxSize);
        REQUIRE( pointer != nullptr );
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == 0 );
        allocator.reset();
        REQUIRE( allocator.getMaxSize() == maxSize );
        REQUIRE( allocator.getAvailableSize() == maxSize );
    }

}

TEST_CASE( "normal cases" ) {

    SECTION( "LinearAllocator works properly for small simple cases" ) {
        simpleTestsForSize(5);
    }

    SECTION( "LinearAllocator works properly for medium simple cases" ) {
        simpleTestsForSize(100 * 1024 * 1024);
    }

    SECTION( "LinearAllocator works properly for large simple cases" ) {
        simpleTestsForSize(1024 * 1024 * 1024);
    }

    SECTION( "LinearAllocator works properly for small loop cases" ) {
        loopTestsForSize(5, 2);
        loopTestsForSize(5, 5);
    }

    SECTION( "LinearAllocator works properly for medium loop cases" ) {
        loopTestsForSize(100 * 1024 * 1024, 100);
        loopTestsForSize(100 * 1024 * 1024, 1024);
    }

    SECTION( "LinearAllocator works properly for large loop case" ) {
        loopTestsForSize(1024 * 1024 * 1024, 10);
        loopTestsForSize(1024 * 1024 * 1024, 1024);
    }

}

