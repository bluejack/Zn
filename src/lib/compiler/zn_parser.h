#ifndef ZINC_PARSER
#define ZINC_PARSER
/*
** parser will take hooks to process complete statements, whether that be 
** compilation to (or toward) machine code, or whether that be a runtime
** for immediate execution.
*/

typedef struct _parser zn_parser;

typedef enum {
  parse_successful = 0,
  parse_unfinished,
  parse_syntax_error
} zn_parse_result;

/*
** Registration function definitions.
*/

typedef (void)(*write_f)(const char*);

/*
** Basic methods
*/

zn_parser *parser_new();

zn_parse_result parse_line(const char* text);

/*
** Registration methods.
*/
void parser_reg_write(write_f);


#endif
