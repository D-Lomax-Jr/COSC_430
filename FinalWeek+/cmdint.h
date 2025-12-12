// cmdint.h - TEFEL-style interface for shell plugin commands

#ifndef CMDINT_H
#define CMDINT_H

typedef void (*cmdint_void_void_f)(void);

// This represents the "cmdint" interface at runtime.
typedef struct {
    cmdint_void_void_f cmd_hello;
    cmdint_void_void_f cmd_date;
} cmdint;

/*
 * cmdint *in = cmdint_bind(char *module, char *errmsg);
 *   Attempt to "bind" lib<module>.so to the cmdint interface:
 *   - Loads lib<module>.so via dlopen
 *   - Locates required symbols cmd_hello and cmd_date (or module_cmd_hello, module_cmd_date)
 *   On success: returns malloc'd cmdint* with function pointers filled in.
 *   On failure: writes error message into errmsg and returns NULL.
 */
cmdint *cmdint_bind(char *module, char *errmsg);

#endif
