/*
 *  f12_any_pkg: test program using the f12 interface binder
 */

#include <stdio.h>
#include <stdlib.h>

#include "f12.h"

int main( int argc, char **argv )
{
    for( int i = 1; argv[i] != NULL; i++ )
    {
        char *module = argv[i];
        char errmsg[1024];

        f12 *f = f12_bind( module, errmsg );
        if( f == NULL )
        {
            fprintf( stderr, "%s\n", errmsg );
        }
        else
        {
            f->f1();
            int n = f->f2();
            printf( "f2 returned %d\n", n );
            free( f );
        }

        printf( "\n" );
    }
    return 0;
}
