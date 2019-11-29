CC=g++

TARGET=allocators_benchmark
TARGET_TEST=allocators_benchmark_test
OPTS=-DNDEBUG -std=c++1y

all: ${TARGET} ${TARGET_TEST}
	./${TARGET_TEST}
	./${TARGET}

run: ${TARGET}
	./${TARGET}

test: ${TARGET_TEST}
	./${TARGET_TEST}

${TARGET}: main.o LinearAllocator.o benchmark.o
	$(CC) -o ${TARGET} main.o LinearAllocator.o benchmark.o ${OPTS}

${TARGET_TEST}: tests.o LinearAllocator.o
	$(CC) -o ${TARGET_TEST} tests.o LinearAllocator.o ${OPTS}

main.o: main.cpp benchmark.h argparse.h
	$(CC) -c main.cpp ${OPTS}

tests.o: tests.cpp LinearAllocator.h catch2/catch.hpp
	$(CC) -c tests.cpp ${OPTS}

LinearAllocator.o: LinearAllocator.cpp LinearAllocator.h
	$(CC) -c LinearAllocator.cpp ${OPTS}

benchmark.o: benchmark.cpp benchmark.h LinearAllocator.h
	$(CC) -c benchmark.cpp ${OPTS}

clean:
	rm -rf *.o ${TARGET} ${TARGET_TEST}
