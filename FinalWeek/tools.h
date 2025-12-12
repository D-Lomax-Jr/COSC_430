#ifndef TOOLS_H
#define TOOLS_H

/* Read a line from stdin, malloc’ing the buffer.
 * Caller must free() the returned string.
 */
char *tools_read_line(void);

/* Print a banner using the given user name (or "user" if NULL). */
void tools_print_banner(const char *username);

/* Print the main menu of shell actions. */
void tools_print_menu(void);

#endif /* TOOLS_H */
