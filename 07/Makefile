
OUTPUT_DIR=build

ifeq ($(OS),Windows_NT)
	CC=C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe
	TARGET=${OUTPUT_DIR}/format.exe
	TARGET_TEST=${OUTPUT_DIR}/format_test.exe
else
	CC=g++
	TARGET=${OUTPUT_DIR}/format
	TARGET_TEST=${OUTPUT_DIR}/format_test
endif

OPTS=-DNDEBUG -std=c++1y

all: ${TARGET} ${TARGET_TEST}
	./${TARGET_TEST}
	./${TARGET}

run: ${TARGET}
	./${TARGET}

test: ${TARGET_TEST}
	./${TARGET_TEST}

${TARGET}: ${OUTPUT_DIR} ${OUTPUT_DIR}/main.o
	$(CC) -o ${TARGET} ${OUTPUT_DIR}/main.o ${OPTS}

${TARGET_TEST}: ${OUTPUT_DIR} ${OUTPUT_DIR}/tests.o
	$(CC) -o ${TARGET_TEST} ${OUTPUT_DIR}/tests.o ${OPTS}

${OUTPUT_DIR}:
	mkdir -p $@

${OUTPUT_DIR}/main.o: main.cpp format.h
	$(CC) -c main.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/tests.o: tests.cpp catch2/catch.hpp format.h
	$(CC) -c tests.cpp -o $@ ${OPTS}

clean:
	rm -rf ${OUTPUT_DIR}/*.o ${TARGET} ${TARGET_TEST}