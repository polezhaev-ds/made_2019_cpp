# made_2019_cpp
MADE: Repository for home assignments (Advanced C++ course)

## Home assignment #7 - Format (exceptions)

### Features
* Format function `Format(formatStr, args...)` was implemented in a way similar to Python/C# formating
* Arguments should be numberted and included in braces, for example: `"{0} + {1} = {3}"`
* According to assignment requirements symbols `'{'` and `'}'` cannot be used without parameter
* Function supports C++ types that can be saved to `std::stringstream` using `<<` operator
* Examples: 
    * `Format(" {0} Bjarne {0} ", "Stroustrup")` returns `" Stroustrup Bjarne Stroustrup "`
    * `Format("{0}, {1}, {2}", 4, 'a', "cdf")` returns `"4, a, cdf"`
* In order to use Format fuction, it neccessary to include `"format.h"` header file
* In the case of errors, fuction raizes `std::runtime_error` exception with appropriate message
* Main functionality is covered by 60 assertions ([Header based catch2 library](https://github.com/catchorg/Catch2))

### How to build and run
* `make` - to build both *format* and *format_test* 
* `make test` - to build and run tests
* `make run` - to build and run a simple sample
* `make clean` - to clean output
* `./build/format` - to run a simple sample
* `./build/format_test` - to run tests

## Home assignment #6 - BigInt class (operations and C++ references)

### Features
* `BigInt` class supports the followint operation signs
    * unary `-`
    * binary `+` and `-` for both `BigInt` objects and `int`
    * comparison operators `<`, `>`, `<=`, `>=`, `==`, `!=` for both `BigInt` objects and `int`
* Different types of references were used in operations to support constant and temporary objects
* `BigInt` is based on the own vector implementation (template class `DynamicArray`) - requirement of assignment
* `BigInt` and `DynamicArray` implement "The rule of 5" (constructor, copy constructor, move constructor, assignment operator, move assignment)
* The size of `BigInt` and `DynamicArray` is limeted by RAM and `std::size_t`
* Main functionality is covered by 191 assertions ([Header based catch2 library](https://github.com/catchorg/Catch2))

### How to build and run
* `make` - to build both *bigint* and *biging_test* 
* `make test` - to build and run tests
* `make run` - to build and run a simple `BigInt` demo
* `make clean` - to clean output
* `./build/bigint` - to run a simple `BigInt` demo
* `./build/bigint_test` - to run `BigInt` tests


## Home assignment #5 - Serialization and templates

### Features
* `Serializer` and `Deserializer` classes are correspodingly for serialization into string and deserialization from it. 
* They support:
    * `ostream`/`istream` classes for storing serialization result or loading object from it 
    * functional operation `()` to pass parameters that are serialized
    * can serializer or deserialize objects of any classes that have at least one field and implement serialize method:
    ~~~
    struct Data {
        std::uint64_t a;
        bool b;
        std::uint64_t c;

        template <class Serializer>
        Error serialize(Serializer& serializer) {
            return serializer(a, b, c); // pass fields to serialize or deserialize here
        }
    
        template <class Serializer>
        Error serialize(Serializer& serializer) const { //this method is required to support constant objects for serialization
            return serializer(a, b, c);
        }
        ...
    }
    ~~~
* Main functionality is covered by 41 assertions ([Header based catch2 library](https://github.com/catchorg/Catch2))

### How to build and run
* `make` - to build both *serialization* and *serialization_test* 
* `make test` - to build and run tests
* `make run` - to build and run silent serialization demo for example from assigment
* `make clean` - to clean output
* `./build/serialization` - to run silent serialization demo
* `./build/serialization_test` - to run serialization tests

## Home assignment #4 - Matrix class

### Features
* `Matrix` class stores integers (`int`) and supports the following methods and operations:
    * Constructor for `std::vector`, for zero-filled `rows` x `columns` matrix
    * Getters for dimensional sizes of matrix
    * Comparison operations: `==` and `!=`
    * Arithmetic operations: `<integer_number> * <matrix>` and `<matrix> * <integer_number>`
    * Assignment operations: `=`, `*= <integer_number>`
    * Indexing using proxy class: `<matrix>[i][j]`
* Matrix class is correctly processed as C++ constant 
* 130 assertions to cover functionality by tests ([Header based catch2 library](https://github.com/catchorg/Catch2))

### How to build and run
* `make` - to build both *matrix* and *matrix_test* (tests for matrix)
* `make test` - to build and run tests
* `make run` - to build and run silent matrix demo for example from assigment and constant example
* `make clean` - to clean output
* `./build/matrix` - to run silent matrix demo
* `./build/matrix_test` - to run run matrix tests

## Home assignment #3 - Tokenizer with callbacks

### Features
* Tokenizer that can parse numerical tokens (sequences of digits), string tokens (sequences of non-whitespaces with at least ony non-digit)
* Tokenizer supports callbacks to handler the following events:
    * Parse was started
    * Parse was finished
    * Next token of any type was parsed
    * Next numerical token was parsed
    * Next string token was parsed
* 205 assertions to cover functionality by tests ([Header based catch2 library](https://github.com/catchorg/Catch2))

### How to build and run
* `make` - to build both *tokenizer* and *tokenizer_test* (tests for tokenizer)
* `make test` - to build and run tests
* `make run ARGS="123   word s324d"` - to build and run tokenizer with a give string
* `make run` - to build and run tokenizer with default string
* `make clean` - to clean output
* `./build/tokenizer "123   word s324d"` - to run tokenizer with a given expression
* `./build/tokenizer_test` - to run all tests

## Home assignment #2 - LinearAllocator

### Features
* LinearAllocator vs malloc comparison benchmark
* 4694 assertions to cover main functionality by tests ([Header based catch2 library](https://github.com/catchorg/Catch2))
* command line arguments parsing ([Header base argparse library](https://github.com/jamolnng/argparse))

### How to build and run
* `make` - to build both *allocators_benchmark* (benchmark to compare LinearAllocator vs malloc) and *allocators_benchmark_test* (to test LinearAllocator implementation)
* `make test` - to build and run tests
* `make run` - to build and run benchmark with default parameters
* `./allocators_benchmark` - to run benchmark with default parameters 
* `./allocators_benchmark [-m DEFAULT_MIN_RANDOM_REQUESTED_SIZE] [-M DEFAULT_MAX_RANDOM_REQUESTED_SIZE] [-r DEFAULT_NUMBER_OF_REQUESTS_PER_TEST] [-t DEFAULT_NUMBER_OF_TESTS] [-w DEFAULT_NUMBER_OF_WARMING_TESTS] ` - to run benchmark with given parameters
* `./allocators_benchmark -h` or `./allocators_benchmark --help` - to see help
* `./allocators_benchmark_test` - to run all tests

## Home assignment #1 - Calculator

### Features of calculator:
* support of 4 mathematical binary operations (+, -, *, /), one unary (-), parentheses, spaces and tabs
* 158 assertions to cover functionality by tests ([Header based catch2 library](https://github.com/catchorg/Catch2))

### How to build and run
* `make` - to build both *calc* (calculator)  and *calc_test* (tests for calculator)
* `make test` - to build and run tests
* `make run ARGS="123+343"` - to build and run calculator with a give expression
* `make run` - to build and run calculator with default expression
* `make clean` - to clean output
* `./calc "123+123"` - to run calculator with a given expression
* `./calc_test` - to run all tests