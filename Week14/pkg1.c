/*
 * pkg1: a collection of C functions
 */

#include <stdio.h>
#include <stdlib.h>

#include "pkg1.h"

void pkg1_f1(void)
{
    printf("pkg1::f1\n");
}

int pkg1_f2(void)
{
    printf("pkg1::f2, returning 1\n");
    return 1;
}

void pkg1_f3(char *s, int x)
{
    printf("pkg1::f3, s='%s', x=%d\n", s, x);
}

void *pkg1_f4(int n)
{
    printf("pkg1::f4, n=%d returning NULL\n", n);
    return NULL;
}
