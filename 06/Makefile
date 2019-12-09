CC=g++

OUTPUT_DIR=build
TARGET=${OUTPUT_DIR}/bigint
TARGET_TEST=${OUTPUT_DIR}/bigint_test
OPTS=-DNDEBUG -std=c++1y


all: ${TARGET} ${TARGET_TEST}
	./${TARGET_TEST}
	./${TARGET}

run: ${TARGET}
	./${TARGET}

test: ${TARGET_TEST}
	./${TARGET_TEST}

${TARGET}: ${OUTPUT_DIR} ${OUTPUT_DIR}/main.o ${OUTPUT_DIR}/BigInt.o
	$(CC) -o ${TARGET} ${OUTPUT_DIR}/main.o ${OUTPUT_DIR}/BigInt.o ${OPTS}

${TARGET_TEST}: ${OUTPUT_DIR} ${OUTPUT_DIR}/tests.o ${OUTPUT_DIR}/BigInt.o
	$(CC) -o ${TARGET_TEST} ${OUTPUT_DIR}/tests.o ${OUTPUT_DIR}/BigInt.o ${OPTS}

${OUTPUT_DIR}:
	mkdir -p $@

${OUTPUT_DIR}/main.o: main.cpp BigInt.h
	$(CC) -c main.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/BigInt.o: BigInt.cpp BigInt.h DynamicArray.h
	$(CC) -c BigInt.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/tests.o: tests.cpp catch2/catch.hpp BigInt.h
	$(CC) -c tests.cpp -o $@ ${OPTS}

clean:
	rm -rf ${OUTPUT_DIR}/*.o ${TARGET} ${TARGET_TEST}
