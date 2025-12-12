// lookup.c - implementation of TEFEL-style symbol lookup

#include "lookup.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

void *lookup_function(pkg_info *info, const char *funcname) {
    // Try plain symbol name first: funcname
    void *p = dlsym(info->dl, funcname);
    if (p != NULL) {
        return p;
    }

    // Try module-qualified name: module_funcname
    char fullname[1024];
    snprintf(fullname, sizeof(fullname), "%s_%s", info->module, funcname);
    p = dlsym(info->dl, fullname);
    if (p != NULL) {
        return p;
    }

    snprintf(info->errmsg, 1024,
             "%s_bind: no symbol '%s' or '%s' in %s",
             info->interface, funcname, fullname, info->libname);
    return NULL;
}
