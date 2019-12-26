
OUTPUT_DIR=build

OUTPUT_NAME=thread_pool

ifeq ($(OS),Windows_NT)
	CC=D:\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\g++.exe
	TARGET=${OUTPUT_DIR}/${OUTPUT_NAME}.exe
	TARGET_TEST=${OUTPUT_DIR}/${OUTPUT_NAME}_test.exe
else
	CC=g++
	TARGET=${OUTPUT_DIR}/${OUTPUT_NAME}
	TARGET_TEST=${OUTPUT_DIR}/${OUTPUT_NAME}_test
endif

OPTS=-DNDEBUG -std=c++17 -pthread

all: ${TARGET} ${TARGET_TEST}
	./${TARGET_TEST}
	./${TARGET}

run: ${TARGET}
	./${TARGET}

test: ${TARGET_TEST}
	./${TARGET_TEST}

${TARGET}: ${OUTPUT_DIR} ${OUTPUT_DIR}/main.o ${OUTPUT_DIR}/ThreadPool.o
	$(CC) -o ${TARGET} ${OUTPUT_DIR}/main.o ${OUTPUT_DIR}/ThreadPool.o ${OPTS}

${TARGET_TEST}: ${OUTPUT_DIR} ${OUTPUT_DIR}/tests.o ${OUTPUT_DIR}/ThreadPool.o
	$(CC) -o ${TARGET_TEST} ${OUTPUT_DIR}/tests.o ${OUTPUT_DIR}/ThreadPool.o  ${OPTS}

${OUTPUT_DIR}:
	mkdir -p $@

${OUTPUT_DIR}/main.o: main.cpp ThreadPool.h
	$(CC) -c main.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/tests.o: tests.cpp catch2/catch.hpp ThreadPool.h
	$(CC) -c tests.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/ThreadPool.o: ThreadPool.cpp ThreadPool.h
	$(CC) -c ThreadPool.cpp -o $@ ${OPTS}

clean:
	rm -rf ${OUTPUT_DIR}/*.o ${TARGET} ${TARGET_TEST}
