# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
AR = ar
ARFLAGS = rcs
TARGET = main

# Source and object files
LIB_SRC = tools.c
LIB_OBJ = $(LIB_SRC:.c=.o)
LIB_NAME = libtools.a

APP_SRC = main.c
APP_OBJ = $(APP_SRC:.c=.o)

# Default target
all: $(LIB_NAME) $(TARGET)

# Create static library (.a)
$(LIB_NAME): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(LIB_OBJ)

# Build main program and link statically
$(TARGET): $(APP_OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $(TARGET) $(APP_OBJ) -L. -ltools

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run program
run: all
	./$(TARGET)

# Clean up
clean:
	rm -f *.o *.a $(TARGET)
