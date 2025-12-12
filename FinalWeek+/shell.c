/***************************************************************************//**

  @file         shell.c
  @brief        TEFEL-enhanced shell with static and dynamic libraries

*******************************************************************************/

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tools.h"
#include "cmdint.h"

/*
  Built-in shell commands:
*/
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_plugin_menu(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
*/
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "plugin"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit,
  &lsh_plugin_menu
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin command implementations.
*/

int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int lsh_help(char **args)
{
  int i;
  printf("TEFEL Shell - built-in commands:\n");
  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }
  printf("Use the 'plugin' command to access plugin menu.\n");
  printf("External programs can also be run.\n");
  return 1;
}

int lsh_exit(char **args)
{
  return 0;
}

/*
  Plugin menu using TEFEL cmdint interface
*/
int lsh_plugin_menu(char **args)
{
  char errmsg[1024];
  cmdint *plugin = cmdint_bind("pkg_shell", errmsg);
  if (plugin == NULL) {
    fprintf(stderr, "Plugin load failed: %s\n", errmsg);
    return 1;
  }

  int choice = 0;
  char line[64];

  while (1) {
    printf("\n--- Plugin Menu (from libpkg_shell.so) ---\n");
    printf("1) Hello\n");
    printf("2) Date\n");
    printf("3) Return to shell\n");
    printf("Select option: ");
    if (!fgets(line, sizeof(line), stdin)) {
      break;
    }
    choice = atoi(line);

    if (choice == 1) {
      plugin->cmd_hello();
    } else if (choice == 2) {
      plugin->cmd_date();
    } else if (choice == 3) {
      break;
    } else {
      printf("Invalid option.\n");
    }
  }

  // In a full TEFEL implementation, you might also dlclose()
  // but here we leave it as-is for simplicity.

  free(plugin);
  return 1;
}

/*
  Launch an external program
*/
int lsh_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/*
  Execute built-in or launch program
*/
int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // Empty command
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}

/*
  Read a line (simple version using getline-like behavior)
*/
char *lsh_read_line(void)
{
  size_t bufsize = 0;
  char *line = NULL;
  ssize_t nread = getline(&line, &bufsize, stdin);
  if (nread == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);
    } else {
      perror("lsh: getline");
      exit(EXIT_FAILURE);
    }
  }
  return line;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/*
  Split a line into tokens
*/
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        free(tokens_backup);
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/*
  Main shell loop
*/
void lsh_loop(void)
{
  char *line;
  char **args;
  int status;

  char user[64] = "user";
  tools_print_banner(user);

  do {
    printf("tefel-shell> ");
    line = lsh_read_line();
    args = lsh_split_line(line);

    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

/*
  main()
*/
int main(int argc, char **argv)
{
  lsh_loop();
  return EXIT_SUCCESS;
}
