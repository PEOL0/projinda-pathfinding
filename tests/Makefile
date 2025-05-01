CC = gcc
CFLAGS = -Wall -Wextra -g -I../include
LDFLAGS = -lcunit -lm

# Source and object files
TEST_SRC = test_pathfinding.c
TEST_OBJ = $(TEST_SRC:.c=.o)
SRC_OBJ = ../src/pathfindning/pathfinding.o

# Target
TARGET = test_pathfinding

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(TEST_OBJ) $(SRC_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Build the pathfinding implementation
../src/pathfindning/pathfinding.o: ../src/pathfindning/pathfinding.c ../include/pathfinding.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Run the tests
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET) $(TEST_OBJ) $(SRC_OBJ)

.PHONY: all run clean