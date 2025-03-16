CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/lexer.c
TEST_SRC = tests/test_lexer.c
OBJ = $(SRC:.c=.o)
TARGET = eaC

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

test-lexer:
	$(CC) $(CFLAGS) $(TEST_SRC) src/lexer.c -o test_lexer && ./test_lexer

clean:
	rm -f $(OBJ) $(TARGET) test_lexer