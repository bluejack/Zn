#ifndef ZINC_DISPATCHER
#define ZINC_DISPATCHER
/*
** The dispatcher is a provisional construct to allow dispatching of basic
** keywords; I expect this will be replaced by a more appropriate symbol
** table.
*/

typedef struct _dispatcher zn_parser;


#include "zn_statement.h"

/*
** Basic methods
*/

zn_dispatcher *dispatcher_new();

/*
** Registration methods.
*/
void dispatcher_reg_write(zn_stmt_handler);


#endif
