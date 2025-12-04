#ifndef LOOKUP_H
#define LOOKUP_H

typedef struct
{
    void *dl;        /* result of dlopen on libname */
    char *module;    /* name of the module we're binding to */
    char *interface; /* name of this interface */
    char *errmsg;    /* pointer to space for error message */
    char *libname;   /* name of the library we're binding to */
} pkg_info;

//
// void *p = lookup_function( pkg_info *info, char *funcname, char *sig );
//
//  Look within info->dl, a dynamic library opened by dlopen(), for
//  either the global symbol <funcname> or the global symbol
//  <modulename_funcname>.
//
//  If we find either, return a pointer to the first one we found.
//  If not, return NULL.
//
//  We also check whether a flag variable corresponding to the given
//  signature (sig) - if it's not NULL - exists in the package, unless
//  a flag variable PKG_useflagvars does not itself exist.
//
void *lookup_function( pkg_info *info, char *funcname, char *sig );

#endif
