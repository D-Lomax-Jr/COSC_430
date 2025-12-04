/*
 *  pkg1: a collection of C functions, that may accidentally satisfy
 *        one or more interfaces that don't yet exist..
 */

#include <stdio.h>
#include <stdlib.h>

char pkg1_f1_void_void;	// pkg1_f1 signature variable:
void pkg1_f1( void )	// pkg1_f1 function:
{
    printf( "pkg1::f1\n" );
}

char pkg1_f2_int_void;	// pkg1_f2 signature variable:
int pkg1_f2( void )	// pkg1_f2 function:
{
    printf( "pkg1::f2, returning 1\n" );
    return 1;
}

char pkg1_f3_void_charstar_int;	// pkg1_f3 signature variable:
void pkg1_f3( char *s, int x )	// pkg1_f3 function:
{
    printf( "pkg1::f3, s='%s', x=%d\n", s, x );
}

char pkg1_f4_voidstar_int;	// pkg1_f4 signature variable:
void *pkg1_f4( int n )	// pkg1_f4 function:
{
    printf( "pkg1::f4, x=%d, returning NULL\n", n );
    return NULL;
}

char pkg1_useflagvars;          // check the existence of the flag variables
