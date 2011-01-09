#ifndef ZN_DISPATCHER
#define ZN_DISPATCHER
/*
** The dispatcher is a provisional construct to allow dispatching of basic
** keywords; I expect this will be replaced by a more appropriate symbol
** table.
*/

typedef struct _dispatcher zn_dispatcher;

#include "zn/zn_statement.h"

typedef void(*zn_output_func)(const char*, ...);

/*
** Basic methods
*/

zn_dispatcher*
dispatcher_new(void);

/*
** Registration methods.
*/
void dsp_reg_write(zn_dispatcher*, zn_stmt_handler);
void dsp_reg_err_writer(zn_dispatcher*, zn_output_func);

/*
** Accessors
*/

zn_stmt_handler dsp_get_write(zn_dispatcher*);
zn_output_func  dsp_get_err_writer(zn_dispatcher*);


#endif
