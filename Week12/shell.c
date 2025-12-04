#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void greet_user(const char *name);

// Trim newline
void strip_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

// Split input into tokens for execvp()
void parse_input(char *input, char **args) {
    char *token = strtok(input, " ");
    int i = 0;

    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

int main() {
    char input[200];
    char name[50];
    char *args[20];

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    strip_newline(name);

    greet_user(name);

    while (1) {
        printf("shell> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            continue;

        strip_newline(input);

        if (strlen(input) == 0)
            continue;

        // Handle built-in exit
        if (strcmp(input, "exit") == 0) {
            printf("Goodbye, %s!\n", name);
            break;
        }

        // Parse user input into argument list
        parse_input(input, args);

        pid_t pid = fork();

        if (pid == 0) {
            execvp(args[0], args);
            perror("Command failed");
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
        }
    }

    return 0;
}
