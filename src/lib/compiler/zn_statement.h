#ifndef ZINC_STATEMENT
#define ZINC_STATEMENT

#include "zinc/zn_param.h"

typedef struct _stmt zn_statement;

typedef char*(*zn_stmt_handler)(zn_statement*);

zn_statement* stmt_new(void);

void stmt_add_handler(zn_statement*, zn_stmt_handler);
void stmt_add_arg(zn_statement*, zn_param*);

zn_param* stmt_next_param(zn_statement*);

char* stmt_process(zn_statement*);

#endif
