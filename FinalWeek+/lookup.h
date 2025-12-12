// lookup.h - helper for TEFEL-style dynamic lookup

#ifndef LOOKUP_H
#define LOOKUP_H

typedef struct {
    void *dl;         // handle from dlopen
    char *module;     // module name (e.g., "pkg_shell")
    char *interface;  // interface name (e.g., "cmdint")
    char *errmsg;     // buffer for error messages
    char *libname;    // library filename (e.g., "libpkg_shell.so")
} pkg_info;

// Look up symbol funcname or module_funcname in the opened library.
// Returns pointer to symbol or NULL on error and sets info->errmsg.
void *lookup_function(pkg_info *info, const char *funcname);

#endif
