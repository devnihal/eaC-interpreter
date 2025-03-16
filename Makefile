CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/lexer.c src/parser.c src/runtime.c
TEST_SRC = tests/test_parser.c
OBJ = $(SRC:.c=.o)
TARGET = eaC

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

test-parser:
	$(CC) $(CFLAGS) $(TEST_SRC) $(SRC) -o test_parser && ./test_parser

clean:
	rm -f $(OBJ) $(TARGET) test_parser