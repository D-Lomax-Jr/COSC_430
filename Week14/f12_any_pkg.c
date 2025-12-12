#include <stdio.h>
#include <stdlib.h>

#include "f12.h"

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        char errmsg[256];
        f12 *iface = f12_bind(argv[i], errmsg);

        if (!iface)
        {
            printf("Error: %s\n", errmsg);
            continue;
        }

        iface->f1();
        int n = iface->f2();
        printf("f2 returned %d\n", n);

        free(iface);
        printf("\n");
    }

    return 0;
}
