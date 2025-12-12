#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "f12.h"
#include "lookup.h"

f12 *f12_bind(char *module, char *errmsg)
{
    char libname[256];
    sprintf(libname, "lib%s.so", module);

    void *dl = dlopen(libname, RTLD_NOW);
    if (!dl)
    {
        sprintf(errmsg, "Cannot dlopen %s", libname);
        return NULL;
    }

    f12 *iface = malloc(sizeof(f12));
    if (!iface)
    {
        strcpy(errmsg, "malloc failed");
        return NULL;
    }

    pkg_info info;
    info.dl = dl;
    info.module = module;
    info.interface = "f12";
    info.errmsg = errmsg;
    info.libname = libname;

    iface->f1 = (f12_void_void_f) lookup_function(&info, "f1");
    if (!iface->f1)
    {
        free(iface);
        return NULL;
    }

    iface->f2 = (f12_int_void_f) lookup_function(&info, "f2");
    if (!iface->f2)
    {
        free(iface);
        return NULL;
    }

    return iface;
}
