#ifndef ZINC_COMMAND
#define ZINC_COMMAND

#include <stdbool.h>

typedef struct _command command;

typedef int(*command_handler)(command*);

command* 
command_new(void);

void     
command_destroy(command*);

void
command_add_arg(command*, char*);

void
command_reset_arglist(command*);

void
command_set_next(command*, command*);

command*
command_get_next(command*);

char*
command_next_arg(command*);

void
command_set_handler(command*, command_handler);

int
command_execute(command*);

bool
command_is_valid(command*);

#endif
