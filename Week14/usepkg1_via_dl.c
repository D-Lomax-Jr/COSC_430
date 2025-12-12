/*
 * Dynamic loading example for pkg1
 */

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef void (*void_void_f)(void);
typedef int  (*int_void_f)(void);

void *lookup_function(void *dl, char *module, char *funcname)
{
    void *p = dlsym(dl, funcname);
    if (p != NULL)
        return p;

    char fullname[256];
    sprintf(fullname, "%s_%s", module, funcname);
    p = dlsym(dl, fullname);

    return p;
}

int main(void)
{
    char *module = "pkg1";
    char libname[256];
    sprintf(libname, "lib%s.so", module);

    void *dl = dlopen(libname, RTLD_NOW);
    if (!dl)
    {
        fprintf(stderr, "Could not dlopen %s\n", libname);
        return 1;
    }

    void_void_f f1 = (void_void_f) lookup_function(dl, module, "f1");
    int_void_f  f2 = (int_void_f)  lookup_function(dl, module, "f2");

    if (!f1 || !f2)
    {
        fprintf(stderr, "Could not find f1 or f2 in module\n");
        return 1;
    }

    f1();
    int n = f2();
    printf("f2 returned %d\n", n);

    return 0;
}
