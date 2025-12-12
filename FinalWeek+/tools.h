// tools.h - helper functions for the shell (static library)

#ifndef TOOLS_H
#define TOOLS_H

#include <stddef.h>

// Print a simple banner for the shell
void tools_print_banner(const char *username);

// Trim trailing newline from a string (if present)
void tools_trim_newline(char *s);

// Simple safe input helper (wrapper over fgets)
char *tools_read_line(char *buf, size_t size);

#endif
