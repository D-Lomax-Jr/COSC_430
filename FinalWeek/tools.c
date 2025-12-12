#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"

#define TOOLS_RL_BUFSIZE 1024

char *tools_read_line(void)
{
    int bufsize = TOOLS_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "tools: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF) {
            exit(EXIT_SUCCESS);
        } else if (c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        if (position >= bufsize) {
            bufsize += TOOLS_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "tools: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void tools_print_banner(const char *username)
{
    if (username == NULL || username[0] == '\0') {
        username = "user";
    }
    printf("=========================================\n");
    printf("  Welcome to the Menu Shell, %s!\n", username);
    printf("  (static + dynamic library demo)\n");
    printf("=========================================\n");
}

void tools_print_menu(void)
{
    printf("\n==== Menu Shell ====\n");
    printf("Choose an action:\n");
    printf("  1) Change directory (cd)\n");
    printf("  2) Show help (help)\n");
    printf("  3) Run an external command\n");
    printf("  4) Use assistant library command\n");
    printf("  5) Exit shell\n");
    printf("====================\n");
}
