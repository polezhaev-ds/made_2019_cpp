# made_2019_cpp
MADE: Repository for home assignments (Advanced C++ course)

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