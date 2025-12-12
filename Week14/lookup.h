#ifndef LOOKUP_H
#define LOOKUP_H

typedef struct {
    void *dl;
    char *module;
    char *interface;
    char *errmsg;
    char *libname;
} pkg_info;

void *lookup_function(pkg_info *info, char *funcname);

#endif
