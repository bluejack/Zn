#ifndef ZINC_SHELL_KEYWORDS
#define ZINC_SHELL_KEYWORDS

#include "command.h"

int zinc_shell_exit(command*);

int zinc_shell_echo(command*);

int zinc_shell_exec(command*);

int zinc_shell_env(command*);

int zinc_shell_assign(command*);

#endif
