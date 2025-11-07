#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>  // For dlopen, dlsym, dlclose

typedef void (*greet_func)(const char *);

int main() {
    int fd[2];
    pid_t pid;
    char name[50];

    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        close(fd[1]); // close write end

        // Read name from parent
        char buffer[50];
        read(fd[0], buffer, sizeof(buffer));
        close(fd[0]);

        // Dynamically load shared library
        void *handle = dlopen("./libtools.so", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "dlopen error: %s\n", dlerror());
            exit(1);
        }

        // Resolve the greet function
        greet_func greet = (greet_func)dlsym(handle, "greet");
        if (!greet) {
            fprintf(stderr, "dlsym error: %s\n", dlerror());
            dlclose(handle);
            exit(1);
        }

        // Call the function
        greet(buffer);

        // Clean up
        dlclose(handle);
        exit(0);
    } else {
        // Parent process
        close(fd[0]); // close read end

        printf("Enter your name: ");
        scanf("%49s", name);

        // Send to child
        write(fd[1], name, strlen(name) + 1);
        close(fd[1]);

        wait(NULL);
    }

    return 0;
}
