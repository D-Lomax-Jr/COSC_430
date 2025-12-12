// tools.c - implementation of helper functions

#include "tools.h"
#include <stdio.h>
#include <string.h>

void tools_print_banner(const char *username) {
    printf("=========================================\n");
    printf("  Welcome to the TEFEL Shell, %s\n", username ? username : "user");
    printf("  Static lib: libtools.a is in use.\n");
    printf("=========================================\n");
}

void tools_trim_newline(char *s) {
    if (!s) return;
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

char *tools_read_line(char *buf, size_t size) {
    if (!fgets(buf, (int)size, stdin)) {
        return NULL;
    }
    tools_trim_newline(buf);
    return buf;
}
