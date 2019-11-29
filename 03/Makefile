CC=g++

OUTPUT_DIR=build
TARGET=${OUTPUT_DIR}/tokenizer
TARGET_TEST=${OUTPUT_DIR}/tokenizer_test
OPTS=-DNDEBUG -std=c++1y


ifeq (${ARGS},)
ARGS=5555 word 3333
endif

all: ${TARGET} ${TARGET_TEST}
	./${TARGET_TEST}
	./${TARGET} "${ARGS}"

run: ${TARGET}
	./${TARGET} "${ARGS}"

test: ${TARGET_TEST}
	./${TARGET_TEST}

${TARGET}: ${OUTPUT_DIR} ${OUTPUT_DIR}/main.o ${OUTPUT_DIR}/Tokenizer.o
	$(CC) -o ${TARGET} ${OUTPUT_DIR}/main.o ${OUTPUT_DIR}/Tokenizer.o ${OPTS}

${TARGET_TEST}: ${OUTPUT_DIR} ${OUTPUT_DIR}/tests.o ${OUTPUT_DIR}/Tokenizer.o ${OUTPUT_DIR}/TokenInfo.o
	$(CC) -o ${TARGET_TEST} ${OUTPUT_DIR}/tests.o ${OUTPUT_DIR}/Tokenizer.o ${OUTPUT_DIR}/TokenInfo.o ${OPTS}

${OUTPUT_DIR}:
	mkdir -p $@

${OUTPUT_DIR}/main.o: main.cpp Tokenizer.h
	$(CC) -c main.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/Tokenizer.o: Tokenizer.cpp Tokenizer.h
	$(CC) -c Tokenizer.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/tests.o: tests.cpp catch2/catch.hpp Tokenizer.h
	$(CC) -c tests.cpp -o $@ ${OPTS}

${OUTPUT_DIR}/TokenInfo.o: TokenInfo.cpp TokenInfo.h
	$(CC) -c TokenInfo.cpp -o $@ ${OPTS}

clean:
	rm -rf ${OUTPUT_DIR}/*.o ${TARGET} ${TARGET_TEST}
