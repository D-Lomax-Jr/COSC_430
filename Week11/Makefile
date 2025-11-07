CC = gcc
CFLAGS = -Wall -Wextra -fPIC -g
LDFLAGS = -ldl
TARGET = main
LIB = libtools.so

all: $(LIB) $(TARGET)

# Build shared library (.so)
$(LIB): tools.o
	$(CC) -shared -o $(LIB) tools.o

tools.o: tools.c
	$(CC) $(CFLAGS) -c tools.c -o tools.o

# Build main program
$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

run: all
	LD_LIBRARY_PATH=. ./$(TARGET)

clean:
	rm -f *.o *.so $(TARGET)
