CC      = gcc
CFLAGS  = -Wall -Wextra -g -fPIC
LDFLAGS = -ldl

all: shell libpkg_shell.so

# --- Static library: libtools.a ---
libtools.a: tools.o
	ar rcs libtools.a tools.o

tools.o: tools.c tools.h
	$(CC) $(CFLAGS) -c tools.c

# --- TEFEL helper + interface ---
lookup.o: lookup.c lookup.h
	$(CC) $(CFLAGS) -c lookup.c

cmdint.o: cmdint.c cmdint.h lookup.h
	$(CC) $(CFLAGS) -c cmdint.c

# --- Plugin shared library ---
pkg_shell.o: pkg_shell.c
	$(CC) $(CFLAGS) -c pkg_shell.c

libpkg_shell.so: pkg_shell.o
	$(CC) -shared -o libpkg_shell.so pkg_shell.o

# --- Shell executable ---
shell.o: shell.c tools.h cmdint.h
	$(CC) $(CFLAGS) -c shell.c

shell: shell.o libtools.a lookup.o cmdint.o
	$(CC) -o shell shell.o lookup.o cmdint.o -L. -ltools $(LDFLAGS)

run: all
	LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH ./shell

clean:
	rm -f *.o *.a *.so shell
