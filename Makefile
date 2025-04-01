CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC_DIR = src
TEST_DIR = tests
INC_DIR = include
OBJ_DIR = obj

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

TARGET = eaC.exe
TEST_TARGET = test_parser.exe

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

test: $(TEST_DIR)/test_parser.c $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $(TEST_TARGET)
	.\$(TEST_TARGET)

clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(TARGET) del $(TARGET)
	if exist $(TEST_TARGET) del $(TEST_TARGET)

.PHONY: all clean test