
#include "command_sequence.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

void _cmd_destroyer(void*);

command_seq* command_seq_new() 
{
  command_seq* seq = ll_new();
  return seq;
}

void _cmd_destroyer(void* obj) {
  command *cmd = (command*)obj;
  command_destroy(cmd);
}

void command_seq_destroy(command_seq *seq)
{
  assert(seq != NULL);
  ll_destroy(seq, &_cmd_destroyer);
}

void command_seq_clear(command_seq *seq)
{
  assert(seq != NULL);
  ll_empty(seq, &_cmd_destroyer);
}

void command_seq_reset(command_seq *seq) {
  assert(seq != NULL);
  ll_reset(seq);
}

bool command_seq_has_next(command_seq *seq) {
  assert(seq != NULL);
  return ll_has_next(seq);
}

void command_seq_add(command_seq *seq, command *cmd) {
  assert(seq != NULL);
  assert(cmd != NULL);
  ll_add(seq, cmd);
}

/*
** Returns:
**   0 on success
**   >0 on failure (return value is exit code)
**   -1 on no further commands to execute.
*/
int command_seq_exec_next(command_seq *seq) {

  assert(seq != NULL);
  command *cmd = (command*)ll_next(seq);
  if (cmd == NULL || !command_is_valid(cmd)) return -1;
  
  return command_execute(cmd);
}
