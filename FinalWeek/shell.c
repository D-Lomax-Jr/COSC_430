/***************************************************************************//**

  @file         shell.c
  @author       <Darryl Lomax Jr>

  @brief        Menu-driven shell using both static and dynamic libraries.

                - Static lib: libtools.a  (banner, menu, line reading)
                - Shared lib: libassistant.so (extra assistant commands)

*******************************************************************************/

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

#include "tools.h"      /* static library */
#include "assistant.h"  /* for type signature only */

/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int lsh_num_builtins(void) {
  return sizeof(builtin_str) / sizeof(char *);
}

/* === Dynamic assistant library support === */

typedef int (*assistant_func_t)(const char *, char **);

static void *assistant_dl = NULL;
static assistant_func_t assistant_handle = NULL;

static void init_assistant(void)
{
    assistant_dl = dlopen("./libassistant.so", RTLD_NOW);
    if (!assistant_dl) {
        fprintf(stderr, "[shell] Warning: could not load libassistant.so: %s\n",
                dlerror());
        return;
    }

    dlerror(); /* clear any existing error */
    assistant_handle = (assistant_func_t)dlsym(assistant_dl,
                                               "assistant_handle_command");
    const char *err = dlerror();
    if (err != NULL) {
        fprintf(stderr, "[shell] Warning: dlsym failed: %s\n", err);
        assistant_handle = NULL;
        dlclose(assistant_dl);
        assistant_dl = NULL;
    } else {
        printf("[shell] Loaded assistant library successfully.\n");
        printf("[shell] Try commands: greet, time, about\n");
    }
}

static void shutdown_assistant(void)
{
    if (assistant_dl != NULL) {
        dlclose(assistant_dl);
        assistant_dl = NULL;
        assistant_handle = NULL;
    }
}

/*
  Builtin function implementations.
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
  (void)args;

  int i;
  printf("=== LSH Menu Shell Help ===\n");
  printf("This shell uses:\n");
  printf("  - a static library (libtools.a) for banner, menu, and input\n");
  printf("  - a shared library (libassistant.so) for extra commands\n\n");

  printf("Built-in commands:\n");
  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("\nAssistant commands (from libassistant.so):\n");
  printf("  greet   - friendly greeting\n");
  printf("  time    - show current time\n");
  printf("  about   - describe the assistant library\n\n");

  printf("You can also run external programs (ls, pwd, mkdir, etc.) via\n");
  printf("menu option 3.\n");
  return 1;
}

int lsh_exit(char **args)
{
  (void)args;
  return 0;
}

/* Launch external (non-builtin, non-assistant) command */
int lsh_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("lsh");
  } else {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/* Dispatch: builtins -> assistant library -> external command */
int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  /* Try assistant library if loaded */
  if (assistant_handle) {
      int handled = assistant_handle(args[0], args);
      if (handled) {
          return 1;
      }
  }

  /* Fallback: run as normal external command */
  return lsh_launch(args);
}

/* Tokenizer */
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

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

/* Menu-driven loop */
void lsh_loop(void)
{
  char *line;
  char **args;
  int status = 1;

  const char *user = getenv("USER");
  tools_print_banner(user);

  while (status) {
    int choice = 0;

    tools_print_menu();
    printf("Enter choice (1-5): ");
    fflush(stdout);

    line = tools_read_line();
    if (sscanf(line, "%d", &choice) != 1) {
      printf("Invalid input. Please enter a number.\n");
      free(line);
      continue;
    }
    free(line);

    switch (choice) {
      case 1: {
        printf("Enter directory to change to: ");
        line = tools_read_line();
        args = malloc(3 * sizeof(char*));
        args[0] = "cd";
        args[1] = line;
        args[2] = NULL;
        status = lsh_execute(args);
        free(line);
        free(args);
        break;
      }

      case 2: {
        args = malloc(2 * sizeof(char*));
        args[0] = "help";
        args[1] = NULL;
        status = lsh_execute(args);
        free(args);
        break;
      }

      case 3: {
        printf("Enter external command (e.g., ls -l):\n> ");
        line = tools_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
        free(line);
        free(args);
        break;
      }

      case 4: {
        printf("Enter assistant command (greet, time, about):\n> ");
        line = tools_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
        free(line);
        free(args);
        break;
      }

      case 5: {
        args = malloc(2 * sizeof(char*));
        args[0] = "exit";
        args[1] = NULL;
        status = lsh_execute(args);
        free(args);
        break;
      }

      default:
        printf("Invalid choice. Please select 1–5.\n");
        break;
    }
  }
}

/* main */
int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  init_assistant();    /* load shared library */
  lsh_loop();          /* run menu-driven shell */
  shutdown_assistant();/* close shared library */

  return EXIT_SUCCESS;
}
