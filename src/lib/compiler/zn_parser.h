#ifndef ZINC_PARSER
#define ZINC_PARSER

#include "zn_statement.h"
#include "zn_dispatcher.h"
#include "zinc/defines.h"

/*
** Given a statement object and a valid dispatcher, this will
** create a valid statement from the text provided, if possible.
*/

zn_result 
parse_line(zn_statement *stmt, zn_dispatcher *dsp, const char* text);

#endif
