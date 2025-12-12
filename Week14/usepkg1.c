#include <stdio.h>
#include "pkg1.h"

int main(void)
{
    pkg1_f1();
    int n = pkg1_f2();
    printf("pkg1_f2 returned %d\n", n);
    pkg1_f3("hello", n);
    pkg1_f4(n);

    return 0;
}
