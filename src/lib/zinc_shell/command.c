
#include "command.h"

#include <stdlib.h>
#include <string.h>

typedef struct _arg {
  struct _arg *next;
  char *val;
} arg;

struct _command {
  struct _command *next;
  command_handler  handler;
  arg             *first;
  arg             *current;
};

command* 
command_new()
{
  command* cmd = malloc(sizeof(command));
  memset(cmd, 0, sizeof(command));
  return cmd;
}

void     
command_destroy(command* cmd) 
{
  while(cmd->first) {
    arg* next = cmd->first->next;
    free(cmd->first->val);
    free(cmd->first);
    cmd->first = next;
  }
  free(cmd);
}

void
command_add_arg(command* cmd, char* argument)
{
  arg* newarg = malloc(sizeof(arg));
  newarg->next = NULL;
  newarg->val  = strdup(argument);
  if (cmd->current) {
    cmd->current->next = newarg;
    cmd->current = newarg;
  } else {
    cmd->first = newarg;
    cmd->current = newarg;
  }
}

void
command_reset_arglist(command* cmd) {
  cmd->current = cmd->first;
}

char*
command_next_arg(command* cmd) {
  arg* retarg = cmd->current;
  if (retarg == NULL) {
    return NULL;
  } else {
    cmd->current = cmd->current->next;
  }
  return retarg->val;
}

void
command_set_handler(command* cmd, command_handler handler) 
{
  cmd->handler = handler;
}

void
command_set_next(command *cmd, command *next) 
{
  cmd->next = next;
}

command*
command_get_next(command *cmd) 
{
  return cmd->next;
}

int
command_execute(command* cmd) 
{
  command_reset_arglist(cmd);
  return cmd->handler(cmd);
}

bool
command_is_valid(command* cmd)
{
  return (cmd->handler != NULL);
}
