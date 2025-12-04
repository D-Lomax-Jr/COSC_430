/*
 *  pkg3: a third package that does NOT implement f2,
 *        so it is not compatible with the f12 interface.
 */

#include <stdio.h>
#include <stdlib.h>

void pkg3_f1( void )
{
    printf( "pkg3::f1\n" );
}

/* No f2() here on purpose */
