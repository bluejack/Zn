#ifndef ZN_COMMAND_SEQUENCE
#define ZN_COMMAND_SEQUENCE

/*---------------------------------------------------------------------------*\
** Command Sequence
**
** Provides facilities for constructing and executing a sequence of commands.
** Currently shell commands do not support any form of branching, so this is
** linear only.
**
** Copyright (c) 2010-2011 Blunt Jackson
**
\*--------------------------------------------------------------------------*/

#include <stdbool.h>

#include "command.h"
#include "zn/zn_list.h"

/*\
 * Opaque Type
\*/
typedef zn_list command_seq;

command_seq* command_seq_new(void);

void command_seq_destroy(command_seq*);

/*\
 * destroys all commands
 *
\*/
void command_seq_clear(command_seq*);

/*\
 * resets execution/addition sequence.
 *
\*/
void command_seq_reset(command_seq*);

void command_seq_add(command_seq*, command*);

bool command_seq_has_next(command_seq*);

/*\
 * Returns:
 *    0 on success
 *   !0 on failure (return value is exit code)
 *   -1 on end-of-sequence (which may be indistinguishable from a command
 *      that returns -1.
\*/
int command_seq_exec_next(command_seq*);

#endif
