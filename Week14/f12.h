#ifndef F12_H
#define F12_H

typedef void (*f12_void_void_f)(void);
typedef int  (*f12_int_void_f)(void);

typedef struct {
    f12_void_void_f f1;
    f12_int_void_f  f2;
} f12;

f12 *f12_bind(char *module, char *errmsg);

#endif
