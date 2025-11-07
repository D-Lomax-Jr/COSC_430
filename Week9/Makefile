# Compiler and flags
CC = gcc
CFLAGS = -Wall -g
TARGET = hello
SRC = hello.c

# Default target
all: $(TARGET)

# Build the program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean compiled files
clean:
	rm -f $(TARGET)
