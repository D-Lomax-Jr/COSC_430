#ifndef ASSISTANT_H
#define ASSISTANT_H

/* Return 1 if command is handled by the assistant, 0 otherwise. */
int assistant_handle_command(const char *cmd, char **args);

#endif /* ASSISTANT_H */
