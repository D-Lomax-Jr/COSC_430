CC      = gcc
CFLAGS  = -Wall -Wextra -g -fPIC
LDFLAGS = -ldl

TARGET  = shell

OBJS    = shell.o
STATIC_LIB  = libtools.a
SHARED_LIB  = libassistant.so

all: $(TARGET)

# Static library from tools.o
$(STATIC_LIB): tools.o
	ar rcs $(STATIC_LIB) tools.o

# Shared library from assistant.o
$(SHARED_LIB): assistant.o
	$(CC) -shared -o $(SHARED_LIB) assistant.o

# Main shell, linked with static & dynamic libs
$(TARGET): $(OBJS) $(STATIC_LIB) $(SHARED_LIB)
	$(CC) -Wall -Wextra -g -o $(TARGET) $(OBJS) -L. -ltools $(LDFLAGS)

# Generic rule for .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	@echo "Remember to set LD_LIBRARY_PATH to include current directory."
	@echo "Example: export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH"
	./$(TARGET)

clean:
	rm -f *.o $(TARGET) $(STATIC_LIB) $(SHARED_LIB)
