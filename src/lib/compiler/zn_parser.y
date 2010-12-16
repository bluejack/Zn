
%union {
  char *text;
};

%token LEFT_PUSH WRITE
%token <text> TERM

%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <stdbool.h>

  #include "zn_parser.h"
  #include "zn_statement.h"
  #include "zn_dispatcher.h"

  extern FILE* yyin;
  extern int   yylineno;
  extern int   yy_scan_string(const char *);
  extern void  yylex_destroy();

  void yyerror(char*);
  int  yylex(void);

  void _prepare_statement();

  /* TODO: Make this a stack so we can recurse into ourself. */
  zn_statement *stmt;
  zn_dispatcher *dsp;
%}

%%

program:
       program statement { _process_statement(); }
     | statement         { _process_statement(); }
     | EOS               { /* empty statement */ }
     ;

statement:
       WRITE LEFT_PUSH TERM 
           { stmt_add_call(stmt, dsp_get_write());
	     stmt_add_arg(stmt, $3);
	   }
     ;
%%

void 
yyerror(char *err) {
  display_err("ERROR: %s\n", err);
}

void _process_statement()  {
  char* err = stmt_process(stmt);
  if (err) yyerror(err);
}

zn_parse_result
parse_line(zn_statement *s, zn_dispatcher *d, const char *text) { 

  stmt = s;
  dsp  = d;

  yy_scan_string(text);
  int result = yyparse();
  yylex_destroy();
  if (result) {
    return zn_parse_failure;
  }
  command_seq_reset(cmd_seq);
  return zn_parse_success;
}
