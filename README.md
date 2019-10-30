# made_2019_cpp
MADE: Repository for home assignments (Advanced C++ course)

# Features of calculator:
* support of 4 mathematical binary operations (+, -, *, /), one unary (-), parentheses, spaces and tabs
* 158 tests to cover functionality ([Header based catch2 library](https://github.com/catchorg/Catch2))

# How to build and run
* `make` - to build both *calc* (calculator)  and *calc_test* (tests for calculator)
* `make test` - to build and run tests
* `make run ARGS="123+343"` - to build and run calculator with a give expression
* `make run` - to build and run calculator with default expression
* `make clean` - to clean output
* `./calc "123+123"` - to run calculator with a given expression
* `./calc_test` - to run all tests

