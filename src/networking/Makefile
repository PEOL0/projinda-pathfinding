CC = gcc
CFLAGS = -Wall -Wextra -g -I../../include -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lws -lm

# Source and object files
SRC = networking.c
OBJ = $(SRC:.c=.o)
PATHFINDING_OBJ = ../pathfindning/pathfinding.o

# Target
TARGET = build/server

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ) $(PATHFINDING_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Build the pathfinding implementation if needed
../pathfindning/pathfinding.o: ../pathfindning/pathfinding.c ../../include/pathfinding.h
	$(MAKE) -C ../pathfindning pathfinding.o

# Run the server
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all run clean