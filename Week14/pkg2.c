/*
 * pkg2: second module implementing functions f1 and f2
 */

#include <stdio.h>
#include <stdlib.h>

void f1(void)
{
    printf("pkg2::f1\n");
}

int f2(void)
{
    int n = 42;
    printf("pkg2::f2, returning %d\n", n);
    return n;
}

void f3(char *s, int x)
{
    printf("pkg2::f3, s='%s', x=%d\n", s, x);
}

void *f4(int n)
{
    printf("pkg2::f4, n=%d returning NULL\n", n);
    return NULL;
}
