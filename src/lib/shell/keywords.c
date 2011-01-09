#include "keywords.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#include "environment.h"
#include "view.h"
#include "shell_kernel.h"
#include "zn/runtime.h"

#define MAX_ARG_LIST 256

/* -------------------------------------------------------------------------- *\
**
** Private procedure declarations
**
\* -------------------------------------------------------------------------- */

char* _find_file_in_path(char* buffer, char* filename);

/* -------------------------------------------------------------------------- *\
**
** Private procedure declarations
**
\* -------------------------------------------------------------------------- */

int zn_shell_exit(command *cmd)
{
  int retval = 0;
  const char* exitval = command_next_arg(cmd);
  if (exitval) {
    retval = atoi(exitval);
  }
  exit_shell(retval);
  return retval; /* Never get here, I should hope! */
}

int zn_shell_echo(command *cmd)
{
  int retval = 0;
  const char* output = command_next_arg(cmd);
  if (output) {
    printline(output);
  } else {
    printline("");
  }
  return retval;
}

int zn_shell_env(command *cmd)
{
  (void) cmd;
  char** envout = zn_env;
  while(*envout) {
    printline(*envout);
    envout++;
  }
  return 0;
}

int zn_shell_assign(command *cmd)
{
  char* key = command_next_arg(cmd);
  char* val = command_next_arg(cmd);
  zn_setenv(key, val, true);
  return 0;
}

int zn_lang_line(command *cmd)
{
  char* line = command_next_arg(cmd);
  runtime_exec_line(line);
  /* TODO: 
   *  - Handle syntax error.
   *  - handle open block.
   */
  return 0;
}

#define INPUT_BUFFER 512

/*
** TODO: Set up two-way communication between shell and child;
**       Right now children are read only.
*/

int zn_shell_exec(command *cmd)
{
  pid_t pid;
  int status = 0;
  char* arglist[MAX_ARG_LIST];
  char* appname = command_next_arg(cmd);
  char  path_buffer[FILENAME_MAX];
  arglist[0] = appname;
  bool wait_for_pid = true;
  
  if (*appname != '/') {
    appname = _find_file_in_path(path_buffer, appname);
    if (!appname) {
      display_err("%s not found", arglist[0]);
      return -1;
    }
  }

  int c = 1;

  while (c < MAX_ARG_LIST) {
    char* arg = (char*)command_next_arg(cmd);
    if (arg && strcmp(arg, "&") == 0) {
      wait_for_pid = false;
    } else {
      arglist[c++] = arg;
    }
    if (arg == NULL) break; /* break after writing null to term the list. */
  }

  /* Special shell view should be suspended until shell resumes. */
  close_view();

  switch(pid = fork()) {
  case -1:
    /* Fork failed! */
    fprintf(stderr, "Failed to execute.\n");
    break;
  case 0: 
    /* Child process */
    status = execve(appname, arglist, zn_env);
    /* If we got here, exec failed. TODO: Figure out why. */
    fprintf(stderr, "Failed to execute %s.\n", appname);
    exit(status);
  default:
    /* Parent */
    if (wait_for_pid) {
      if (waitpid(pid, &status, 0) < 0) {
	/* Wait for pid failed!! */
	fprintf(stderr, "System error tracking process %s\n", appname);
      }
      if ( !WIFEXITED(status)) {      
	fprintf(stderr, "Debug, system error -- exit failure?\n");
      }
    } else {
      printf("Started Process %d\n", pid);
    }

    initialize_view();

  }

  return status;
}

/* -------------------------------------------------------------------------- *\
**
** Private procedure implementations
**
\* -------------------------------------------------------------------------- */

char* 
_find_file_in_path(char *buffer, char *filename) {

  int c = 0;
  int i = 0;
  const char* path = zn_getenv("PATH");
  
  /* move pointer to first char after = sign */
  while (*path != '\0') {
    while (*path != ':' && *path != '\0') {
      buffer[c++] = *path;
      if (c == FILENAME_MAX) break;
      path++;
    }
    if (c == FILENAME_MAX) break;
    buffer[c++] = '/';
    path++;
    while (c < FILENAME_MAX) {
      buffer[c++] = filename[i];
      if (filename[i] == '\0') break;
      i++;
    }
    if (c < FILENAME_MAX) {
      struct stat fileinfo;
      if (stat(buffer, &fileinfo) == -1) {
	/* Try the next one. */
      } else {
	return buffer;
      }
      /* TODO: Test executability! */
    }
    if (*path == ':') path++;
    c = 0;
    i = 0;
  }
  return NULL;
}
    
