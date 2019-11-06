# made_2019_cpp
MADE: Repository for home assignments (Advanced C++ course)

## Home assignment #2 - LinearAllocator

### Features
* LinearAllocator vs malloc comparison benchmark
* 4694 assertions to cover main functionality ([Header based catch2 library](https://github.com/catchorg/Catch2))
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
* 158 tests to cover functionality ([Header based catch2 library](https://github.com/catchorg/Catch2))

### How to build and run
* `make` - to build both *calc* (calculator)  and *calc_test* (tests for calculator)
* `make test` - to build and run tests
* `make run ARGS="123+343"` - to build and run calculator with a give expression
* `make run` - to build and run calculator with default expression
* `make clean` - to clean output
* `./calc "123+123"` - to run calculator with a given expression
* `./calc_test` - to run all tests