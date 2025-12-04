#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dlfcn.h>

#include "f12.h"
#include "lookup.h"



/*
 * f12 *in = f12_bind( char *module, char *errmsg );
 *  Attempt to "bind" lib<module>.so to the f12 interface:
 *  Load "lib<module>.so" into memory, and attempt to locate all the
 *  required function symbols inside the library. For each function
 *  called <fname>, we look first for a symbol "fname", then if that fails,
 *  for a symbol "module_fname".
 *
 *  If we fail to find even one of the required functions: strcpy
 *  an error message into errmsg and return NULL
 *
 *  If we succeed then we say we have "bound" the module to the interface:
 *  we return an newly malloc()d f12 object with the slot function
 *  pointers bound to the corresponding functions in lib<module>.so
 */
f12 *f12_bind( char *module, char *errmsg )
{
    char libname[1024];
    assert( strlen(module) < 1000 );
    sprintf( libname, "lib%s.so", module );
    void *dl = dlopen( libname, RTLD_NOW );
    if( dl == NULL )
    {
        sprintf( errmsg, "f12_bind: dlopen of %s failed", libname );
        return NULL;
    }

    f12 *in = malloc(sizeof(*in));
    if( in == NULL )
    {
        strcpy( errmsg, "f12_bind: malloc() failed" );
        return NULL;
    }

    pkg_info   info;
    info.dl        = dl;
    info.module    = module;
    info.interface = "f12";
    info.libname   = libname;
    info.errmsg    = errmsg;

    in->f1 = (f12_void_void_f) lookup_function( &info, "f1", "f1_void_void" );
    if( in->f1 == NULL )
    {
        free(in);
        return NULL;
    }

    in->f2 = (f12_int_void_f) lookup_function( &info, "f2", "f2_int_void" );
    if( in->f2 == NULL )
    {
        free(in);
        return NULL;
    }

    return in;
}
