#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef void (*void_void_f)(void);
typedef int  (*int_void_f)(void);

void call_pkg(char *module)
{
    char libname[256];
    sprintf(libname, "lib%s.so", module);

    void *dl = dlopen(libname, RTLD_NOW);
    if (!dl)
    {
        fprintf(stderr, "Cannot dlopen %s\n", libname);
        return;
    }

    char fullname1[256], fullname2[256];
    sprintf(fullname1, "%s_f1", module);
    sprintf(fullname2, "%s_f2", module);

    void_void_f f1 = (void_void_f) dlsym(dl, "f1");
    if (!f1) f1 = (void_void_f) dlsym(dl, fullname1);

    int_void_f f2 = (int_void_f) dlsym(dl, "f2");
    if (!f2) f2 = (int_void_f) dlsym(dl, fullname2);

    if (!f1 || !f2)
    {
        fprintf(stderr, "Module %s is NOT compatible\n", module);
        return;
    }

    printf("Calling %s::f1\n", module);
    f1();

    int r = f2();
    printf("%s::f2 returned %d\n", module, r);
}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
        call_pkg(argv[i]);

    return 0;
}
