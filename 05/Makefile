CC=g++

OUTPUT_DIR=build
TARGET=${OUTPUT_DIR}/serialization
TARGET_TEST=${OUTPUT_DIR}/serialization_test
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

${OUTPUT_DIR}/main.o: main.cpp data.h serialization.h
	$(CC) -c main.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/tests.o: tests.cpp catch2/catch.hpp serialization.h test_helpers.h
	$(CC) -c tests.cpp -o $@ ${OPTS}

clean:
	rm -rf ${OUTPUT_DIR}/*.o ${TARGET} ${TARGET_TEST}
