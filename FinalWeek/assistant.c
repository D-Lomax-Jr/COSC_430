#include <stdio.h>
#include <string.h>
#include <time.h>

#include "assistant.h"

int assistant_handle_command(const char *cmd, char **args)
{
    (void)args; /* not used yet, but kept for future extensions */

    if (cmd == NULL) {
        return 0;
    }

    if (strcmp(cmd, "greet") == 0) {
        printf("[assistant.so] Hello! I am your helper library.\n");
        printf("[assistant.so] Try 'time' or 'about' as well.\n");
        return 1;
    }

    if (strcmp(cmd, "time") == 0) {
        time_t now = time(NULL);
        if (now == (time_t)-1) {
            perror("[assistant.so] time");
            return 1;
        }
        char buf[128];
        struct tm *tm_info = localtime(&now);
        if (tm_info && strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info)) {
            printf("[assistant.so] Current local time: %s\n", buf);
        } else {
            printf("[assistant.so] Could not format time.\n");
        }
        return 1;
    }

    if (strcmp(cmd, "about") == 0) {
        printf("[assistant.so] This command is implemented in a shared library.\n");
        printf("[assistant.so] The shell loads me with dlopen() / dlsym().\n");
        return 1;
    }

    /* Not an assistant command */
    return 0;
}
