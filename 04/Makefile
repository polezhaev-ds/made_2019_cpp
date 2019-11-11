CC=g++

OUTPUT_DIR=build
TARGET=${OUTPUT_DIR}/matrix
TARGET_TEST=${OUTPUT_DIR}/matrix_test
OPTS=-DNDEBUG -std=c++1y


all: ${TARGET} ${TARGET_TEST}
	./${TARGET_TEST}
	./${TARGET}

run: ${TARGET}
	./${TARGET}

test: ${TARGET_TEST}
	./${TARGET_TEST}

${TARGET}: ${OUTPUT_DIR} ${OUTPUT_DIR}/main.o ${OUTPUT_DIR}/Matrix.o
	$(CC) -o ${TARGET} ${OUTPUT_DIR}/main.o ${OUTPUT_DIR}/Matrix.o ${OPTS}

${TARGET_TEST}: ${OUTPUT_DIR} ${OUTPUT_DIR}/tests.o ${OUTPUT_DIR}/Matrix.o
	$(CC) -o ${TARGET_TEST} ${OUTPUT_DIR}/tests.o ${OUTPUT_DIR}/Matrix.o ${OPTS}

${OUTPUT_DIR}:
	mkdir -p $@

${OUTPUT_DIR}/main.o: main.cpp Matrix.h
	$(CC) -c main.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/Matrix.o: Matrix.cpp Matrix.h
	$(CC) -c Matrix.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/tests.o: tests.cpp catch2/catch.hpp Matrix.h
	$(CC) -c tests.cpp -o $@ ${OPTS}

clean:
	rm -rf ${OUTPUT_DIR}/*.o ${TARGET} ${TARGET_TEST}
