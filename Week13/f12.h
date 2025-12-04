typedef void (*f12_void_void_f)( void );
typedef int (*f12_int_void_f)( void );

// This represents the "interface f12" at run-time.
// It's a container of SLOTS for the f12 functions..
typedef struct
{
	f12_void_void_f f1;
	f12_int_void_f f2;
} f12;


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
extern f12 *f12_bind( char *module, char *errmsg );
