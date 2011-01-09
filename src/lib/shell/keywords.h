#ifndef ZN_SHELL_KEYWORDS
#define ZN_SHELL_KEYWORDS

#include "command.h"

int zn_shell_exit(command*);

int zn_shell_echo(command*);

int zn_shell_exec(command*);

int zn_shell_env(command*);

int zn_shell_assign(command*);

int zn_lang_line(command*);

#endif
