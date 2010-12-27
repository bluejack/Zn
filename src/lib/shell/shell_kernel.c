
#include "shell_kernel.h"

#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "term.h"
#include "ncurses.h"

#include "environment.h"
#include "view.h"

typedef struct _shell {
  int last_exit;
} zinc_shell;

zinc_shell shell;

void initialize_shell(char **env) {

  /* Set up state. */
  memset(&shell, 0, sizeof(zinc_shell));

  /* Merge parent environment */
  zinc_create_env(env);

  /* TODO: Handle configuration */

  /* Prepare UI */
  initialize_view();
}

void exit_shell(int exit_val) {
  /* Shutdown UI */
  close_view();
  
  /* Terminate application */
  exit(exit_val);

}

void set_last_exit(int exitval) {
  shell.last_exit = exitval;
}

int  get_last_exit() {
  return shell.last_exit;
}
