#include <stdio.h>

void greet(const char *name) {
    printf("Hello, %s! (from dynamically linked library)\n", name);
}
