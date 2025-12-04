/*
 * lookup: helper to search for a function symbol in a dl
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "lookup.h"

//
// void *p = lookup_function( pkg_info *info, char *funcname, char *sig );
//  look within info->dl, a dynamic library opened by dlopen(), for
//  either the global symbol <funcname> or the global symbol
//  <modulename_funcname>.
//  If we find either, return a pointer to the first one we found.
//  If not, return NULL.
//  We also check whether a flag variable corresponding to the given
//  signature (sig) - if it's not NULL - exists in the package, unless
//  a flag variable PKG_useflagvars does not itself exist.
//
void *lookup_function( pkg_info *info, char *funcname, char *sig )
{
    // can we look up the unadorned symbol funcname inside the dl?
    void *p = dlsym( info->dl, funcname );
    if( p == NULL )
    {
        // can we look up the module-qualified symbol inside the dl?
        char fullname[1024];
        sprintf( fullname, "%s_%s", info->module, funcname );
        p = dlsym( info->dl, fullname );
        if( p == NULL )
        {
            sprintf( info->errmsg,
                     "No symbol '%s' or '%s' in %s",
                     funcname, fullname, info->libname );
            return NULL;
        }
    }

    char useflags[1024];
    sprintf( useflags, "%s_useflagvars", info->module );

    // Should we check for a signature flag variable?
    if( sig != NULL && dlsym( info->dl, useflags ) != NULL )
    {
        // check the flag variable for the signature
        char sigsym[1024];
        sprintf( sigsym, "%s_%s", info->module, sig );

        // if the signature symbol doesn't exist, fail..
        if( dlsym( info->dl, sigsym ) == NULL )
        {
            sprintf( info->errmsg,
                     "%s_bind: No sig symbol '%s' in %s",
                     info->interface, sigsym, info->libname );
            return NULL;
        }
    }

    return p;
}
