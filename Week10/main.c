#include <stdio.h>

// Declaration of the function in the static library
void greet(const char *name);

int main() {
    char name[50];

    printf("Enter your name: ");
    scanf("%49s", name); // Read user input safely

    greet(name); // Call the library function

    return 0;
}
