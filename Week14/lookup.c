#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "lookup.h"

void *lookup_function(pkg_info *info, char *funcname)
{
    void *p = dlsym(info->dl, funcname);
    if (p != NULL)
        return p;

    char fullname[256];
    sprintf(fullname, "%s_%s", info->module, funcname);

    p = dlsym(info->dl, fullname);
    if (p != NULL)
        return p;

    sprintf(info->errmsg,
            "No symbol %s or %s in %s",
            funcname, fullname, info->libname);

    return NULL;
}
