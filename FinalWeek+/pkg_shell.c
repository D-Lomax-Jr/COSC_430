// pkg_shell.c - plugin module implementing cmdint interface

#include <stdio.h>
#include <stdlib.h>

void cmd_hello(void) {
    printf("[plugin] Hello from libpkg_shell.so!\n");
}

void cmd_date(void) {
    printf("[plugin] Current date/time from plugin:\n");
    // Use system date just for demonstration
    system("date");
}
