CC=g++

TARGET=calc
TARGET_TEST=calc_test
OPTS=-DNDEBUG -std=c++1y

ifeq (${ARGS},)
ARGS="123+345*233"
endif

all: ${TARGET} ${TARGET_TEST}
	./${TARGET_TEST}
	./${TARGET} "${ARGS}"

run: ${TARGET}
	./${TARGET} "${ARGS}"

test: ${TARGET_TEST}
	./${TARGET_TEST}

${TARGET}: main.o Parser.o SyntaxAnalyzer.o LexicalAnalyzer.o Token.o NumberToken.o Exception.o AnalyzerException.o
	$(CC) -o ${TARGET} main.o Parser.o SyntaxAnalyzer.o LexicalAnalyzer.o Token.o NumberToken.o Exception.o AnalyzerException.o ${OPTS}

${TARGET_TEST}: tests.o Parser.o SyntaxAnalyzer.o LexicalAnalyzer.o Token.o NumberToken.o Exception.o AnalyzerException.o
	$(CC) -o ${TARGET_TEST} tests.o Parser.o SyntaxAnalyzer.o LexicalAnalyzer.o Token.o NumberToken.o Exception.o AnalyzerException.o ${OPTS}

main.o: main.cpp Parser.h Exception.h AnalyzerException.h
	$(CC) -c main.cpp ${OPTS}

tests.o: tests.cpp catch2/catch.hpp Parser.h
	$(CC) -c tests.cpp ${OPTS}

Parser.o: Parser.cpp Parser.h AnalyzerException.h LexicalAnalyzer.h SyntaxAnalyzer.h
	$(CC) -c Parser.cpp ${OPTS}

SyntaxAnalyzer.o: SyntaxAnalyzer.cpp SyntaxAnalyzer.h Token.h NumberToken.h AnalyzerException.h
	$(CC) -c SyntaxAnalyzer.cpp ${OPTS}

LexicalAnalyzer.o: LexicalAnalyzer.cpp LexicalAnalyzer.h Token.h AnalyzerException.h
	$(CC) -c LexicalAnalyzer.cpp ${OPTS}

Token.o: Token.cpp Token.h NumberToken.h Exception.h
	$(CC) -c Token.cpp ${OPTS}

NumberToken.o: NumberToken.cpp NumberToken.h Token.h
	$(CC) -c NumberToken.cpp ${OPTS}

Exception.o: Exception.cpp Exception.h
	$(CC) -c Exception.cpp ${OPTS}

AnalyzerException.o: AnalyzerException.cpp AnalyzerException.h Exception.h
	$(CC) -c AnalyzerException.cpp ${OPTS}

clean:
	rm -rf *.o ${TARGET} ${TARGET_TEST}


