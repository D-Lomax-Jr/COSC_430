CC      = gcc
CFLAGS  = -Wall -Wextra -fPIC -g

ALL_BINS = f12_any_pkg
ALL_LIBS = libpkg1.so libpkg2.so libpkg3.so

all: $(ALL_LIBS) $(ALL_BINS)

#
# Generate pkg1.c from pkg1.pkg using cpkg
#
pkg1.c: pkg1.pkg cpkg Support.pm Sig.pm
	perl ./cpkg pkg1.pkg

#
# Shared libraries for the packages
#
libpkg1.so: pkg1.c
	$(CC) $(CFLAGS) -c pkg1.c -o pkg1.o
	$(CC) -shared -o libpkg1.so pkg1.o

libpkg2.so: pkg2.c
	$(CC) $(CFLAGS) -c pkg2.c -o pkg2.o
	$(CC) -shared -o libpkg2.so pkg2.o

libpkg3.so: pkg3.c
	$(CC) $(CFLAGS) -c pkg3.c -o pkg3.o
	$(CC) -shared -o libpkg3.so pkg3.o

#
# Generate f12.c and f12.h from f12.interface using cint
#
f12.c f12.h: f12.interface cint Support.pm Sig.pm
	perl ./cint f12.interface

#
# Lookup + interface objects
#
lookup.o: lookup.c lookup.h
	$(CC) $(CFLAGS) -c lookup.c -o lookup.o

f12.o: f12.c f12.h lookup.h
	$(CC) $(CFLAGS) -c f12.c -o f12.o

f12_any_pkg.o: f12_any_pkg.c f12.h
	$(CC) $(CFLAGS) -c f12_any_pkg.c -o f12_any_pkg.o

f12_any_pkg: f12_any_pkg.o f12.o lookup.o
	$(CC) -o f12_any_pkg f12_any_pkg.o f12.o lookup.o -ldl

.PHONY: clean run

clean:
	rm -f *.o *.so $(ALL_BINS) pkg1.c f12.c f12.h

run: all
	@echo "Remember to set LD_LIBRARY_PATH, e.g.:"
	@echo "  export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH"
	@echo
	@echo "Then run:"
	@echo "  ./f12_any_pkg pkg1 pkg2 pkg3"

