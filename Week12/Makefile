# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
AR = ar
ARFLAGS = rcs
TARGET = shell

# Static library files
LIB_SRC = tools.c
LIB_OBJ = $(LIB_SRC:.c=.o)
LIB_NAME = libtools.a

# Shell source and object files
SRC = shell.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(LIB_NAME) $(TARGET)

# Build static library
$(LIB_NAME): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(LIB_OBJ)

# Build shell program (static linking)
$(TARGET): $(OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -L. -ltools

# Compile C files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the shell
run: all
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f *.o *.a $(TARGET)
