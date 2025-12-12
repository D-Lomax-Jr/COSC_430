#include <stdio.h>      // for printf, scanf, perror
#include <stdlib.h>     // for exit, malloc, etc.
#include <unistd.h>     // for fork, exec, pipe, read, write
#include <string.h>     // for strlen
#include <sys/types.h>  // for pid_t
#include <sys/wait.h>   // for wait

int main(void) {
    char name[50];   // buffer to store user’s name
    int choice;      // menu selection

    // Ask the user to input their name
    printf("Welcome! Please enter your name: ");
    scanf("%49s", name);                  // read up to 49 chars (leave space for '\0')
    while (getchar() != '\n');            // flush leftover newline from input buffer

    // Infinite loop until user chooses Exit
    while (1) {
        // Display menu
        printf("\n--- Mini Shell Menu ---\n");
        printf("Hello, %s!\n", name);
        printf("1. List files (ls -l)\n");
        printf("2. Make a new directory (mkdir)\n");
        printf("3. Demonstrate pipe (send message to child)\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        // Read menu choice
        if (scanf("%d", &choice) != 1) {      // input validation
            printf("Invalid input. Try again.\n");
            while (getchar() != '\n');        // clear invalid input from buffer
            continue;                         // restart loop
        }
        while (getchar() != '\n');            // flush newline after number

        // Option 4: Exit program
        if (choice == 4) {
            printf("Goodbye, %s!\n", name);
            break;   // exit the infinite loop
        }

        pid_t pid;  // process ID
        int fd[2];  // pipe file descriptors (fd[0] = read end, fd[1] = write end)

        switch (choice) {
            case 1: // Run "ls -l"
                pid = fork();    // create child process
                if (pid == 0) {
                    // Child process replaces itself with "ls -l"
                    execlp("ls", "ls", "-l", NULL);
                    perror("exec failed");    // exec only returns on error
                    exit(1);
                } else {
                    // Parent waits for child to finish
                    wait(NULL);
                }
                break;

            case 2: { // Run "mkdir"
                char dirname[50];
                printf("Enter directory name: ");
                scanf("%49s", dirname);
                while (getchar() != '\n');    // flush newline

                pid = fork();
                if (pid == 0) {
                    // Child runs mkdir <dirname>
                    execlp("mkdir", "mkdir", dirname, NULL);
                    perror("exec failed");
                    exit(1);
                } else {
                    // Parent waits for child
                    wait(NULL);
                }
                break;
            }

            case 3: // Demonstrate pipe
                if (pipe(fd) == -1) {     // create pipe
                    perror("pipe failed");
                    exit(1);
                }
                pid = fork();             // create child
                if (pid < 0) {
                    perror("fork failed");
                    exit(1);
                }
                if (pid == 0) {
                    // --- Child process ---
                    close(fd[1]);         // close unused write end
                    char buffer[100];
                    // Read message from parent through pipe
                    read(fd[0], buffer, sizeof(buffer));
                    printf("Child received message: %s\n", buffer);
                    close(fd[0]);         // close read end
                    exit(0);
                } else {
                    // --- Parent process ---
                    close(fd[0]);         // close unused read end
                    char message[100] = "Hello from parent process!";
                    // Write message to pipe
                    write(fd[1], message, strlen(message) + 1);
                    close(fd[1]);         // close write end
                    wait(NULL);           // wait for child to finish
                }
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;   // program ends
}
