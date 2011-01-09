#ifndef ZN_STATEMENT
#define ZN_STATEMENT

#include "zn/zn_param.h"

typedef struct _stmt zn_statement;

typedef char*(*zn_stmt_handler)(zn_statement*);

zn_statement* stmt_new(void);
void stmt_destroy(zn_statement*);

void stmt_add_handler(zn_statement*, zn_stmt_handler);
void stmt_add_arg(zn_statement*, zn_param*);

void      stmt_reset(zn_statement*);
zn_param* stmt_next_param(zn_statement*);

char* stmt_process(zn_statement*);

#endif
