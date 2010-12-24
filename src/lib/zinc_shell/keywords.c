#include "keywords.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "environment.h"
#include "view.h"
#include "shell_kernel.h"

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

int zinc_shell_exit(command *cmd)
{
  int retval = 0;
  const char* exitval = command_next_arg(cmd);
  if (exitval) {
    retval = atoi(exitval);
  }
  exit_shell(retval);
  return retval; /* Never get here, I should hope! */
}

int zinc_shell_echo(command *cmd)
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

int zinc_shell_env(command *cmd)
{
  (void) cmd;
  char** envout = zinc_env;
  while(*envout) {
    printline(*envout);
    envout++;
  }
  return 0;
}

int zinc_shell_assign(command *cmd)
{
  char* key = command_next_arg(cmd);
  char* val = command_next_arg(cmd);
  zinc_setenv(key, val, true);
  return 0;
}

int zinc_lang_line(command *cmd)
{
  char* line = command_next_arg(cmd);
  display_text("ZINC LANG: %s\n", line);
  return 0;
}

#define INPUT_BUFFER 512

/*
** TODO: Set up two-way communication between shell and child;
**       Right now children are read only.
*/

int zinc_shell_exec(command *cmd)
{
  pid_t pid;
  int status = 0;
  char* arglist[MAX_ARG_LIST];
  char* appname = command_next_arg(cmd);
  char  path_buffer[FILENAME_MAX];
  arglist[0] = appname;
  
  if (*appname != '/') {
    appname = _find_file_in_path(path_buffer, appname);
    if (!appname) {
      display_err("%s not found", arglist[0]);
      return -1;
    }
  }

  int pipes[2];
  int c = 1;
  if (pipe(pipes) == -1) {
    display_err("Failed establishing communication to process.");
    return 1;
  }

  while (c < MAX_ARG_LIST) {
    char* arg = (char*)command_next_arg(cmd);
    arglist[c++] = arg;
    if (arg == NULL) break; /* break after writing null to term the list. */
  }

  switch(pid = fork()) {
  case -1:
    /* Fork failed! */
    display_err("System error allocating an additional process.\n");
    break;
  case 0: 
    /* Child process */
    close(pipes[0]);
    dup2(pipes[1], STDOUT_FILENO);
    status = execve(appname, arglist, zinc_env);
    /* If we got here, exec failed. TODO: Figure out why. */
    display_err("Failed to execute %s, reason TBD\n", appname);
    exit(status);
  default:
    /* Parent */
    close(pipes[1]);
    FILE *stream = fdopen(pipes[0], "r");
    char buffer[INPUT_BUFFER];
    int i = 0, next;
    while ( ( next = fgetc( stream ) ) != EOF ) {
      buffer[i++] = next;
      if (next == '\n' || c == INPUT_BUFFER - 1) {
	buffer[i] = '\0';
	display_text(buffer);
	i = 0;
      }
    }
    if (i > 0) {
      buffer[i] = '\0';
      display_text(buffer);
    }
    
    close(pipes[0]);
    
    if (waitpid(pid, &status, 0) < 0) {
      /* Wait for pid failed!! */
      display_err("System error tracking process %s\n", appname);
    }

    if ( !WIFEXITED(status)) {      
      display_err("Debug, system error -- exit failure?\n");
    }
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
  const char* path = zinc_getenv("PATH");
  
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
    
