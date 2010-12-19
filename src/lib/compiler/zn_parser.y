
%union {
  char *text;
};

%token LEFT_PUSH WRITE EOS
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

  void _process_statement(void);

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
           { stmt_add_handler(stmt, dsp_get_write(dsp));
	     zn_param* p = param_new();
	     param_set(p, $3);
	     stmt_add_arg(stmt, p);
	   }
     ;
%%

void 
yyerror(char *err) {
  zn_output_func errout = dsp_get_err_writer(dsp);
  errout("ERROR: %s\n", err);
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
  return zn_parse_successful;
}
