CC=gcc
CFLAGS=-Wall -Wextra -fPIC -g

all: pkg1 pkg2 pkg3 interfaces tests

#
# Shared libraries
#
pkg1:
	$(CC) $(CFLAGS) -c pkg1.c
	$(CC) -shared -o libpkg1.so pkg1.o

pkg2:
	$(CC) $(CFLAGS) -c pkg2.c
	$(CC) -shared -o libpkg2.so pkg2.o

pkg3:
	$(CC) $(CFLAGS) -c pkg3.c
	$(CC) -shared -o libpkg3.so pkg3.o

#
# Interface and lookup code
#
interfaces:
	$(CC) $(CFLAGS) -c lookup.c
	$(CC) $(CFLAGS) -c f12.c

#
# Test programs
#
tests:
	$(CC) -o usepkg1 usepkg1.c pkg1.o
	$(CC) -o usepkg1_via_dl usepkg1_via_dl.c -ldl
	$(CC) -o use_any_pkg use_any_pkg.c -ldl

	$(CC) -o f12_any_pkg f12_any_pkg.c f12.o lookup.o -ldl

clean:
	rm -f *.o *.so usepkg1 usepkg1_via_dl use_any_pkg f12_any_pkg
