#ifndef ZINC_PARSER
#define ZINC_PARSER

#include "zn_statement.h"
#include "zn_dispatcher.h"

typedef enum {
  zn_parse_successful = 0,
  zn_parse_incomplete,
  zn_parse_failure,
} zn_parse_result;

/*
** Given a statement object and a valid dispatcher, this will
** create a valid statement from the text provided, if possible.
*/

zn_parse_result 
parse_line(zn_statement *stmt, zn_dispatcher *dsp, const char* text);

#endif
