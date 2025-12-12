// cmdint.c - TEFEL-style binder for cmdint interface

#include "cmdint.h"
#include "lookup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dlfcn.h>

cmdint *cmdint_bind(char *module, char *errmsg) {
    char libname[1024];
    snprintf(libname, sizeof(libname), "lib%s.so", module);

    void *dl = dlopen(libname, RTLD_NOW);
    if (dl == NULL) {
        snprintf(errmsg, 1024, "cmdint_bind: dlopen of %s failed: %s",
                 libname, dlerror());
        return NULL;
    }

    cmdint *in = malloc(sizeof(*in));
    if (in == NULL) {
        snprintf(errmsg, 1024, "cmdint_bind: malloc() failed");
        return NULL;
    }

    pkg_info info;
    info.dl        = dl;
    info.module    = module;
    info.interface = "cmdint";
    info.libname   = libname;
    info.errmsg    = errmsg;

    // Look up cmd_hello
    in->cmd_hello = (cmdint_void_void_f)lookup_function(&info, "cmd_hello");
    if (in->cmd_hello == NULL) {
        free(in);
        return NULL;
    }

    // Look up cmd_date
    in->cmd_date = (cmdint_void_void_f)lookup_function(&info, "cmd_date");
    if (in->cmd_date == NULL) {
        free(in);
        return NULL;
    }

    return in;
}
