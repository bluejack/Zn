#ifndef ZINC_STATEMENT
#define ZINC_STATEMENT

typedef struct _stmt zn_statement;

typedef const char*(*zn_stmt_handler)(zn_statement*);

zn_statement* stmt_new();

void stmt_add_handler(zn_statement*, zn_stmt_handler);
void stmt_add_arg(zn_statement*, zn_arg*);

const char* stmt_process(zn_statement*);

#endif
