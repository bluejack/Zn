
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

  extern FILE* znin;
  extern int   znlineno;
  extern int   zn_scan_string(const char *);
  extern void  znlex_destroy();

  void znerror(char*);
  int  znlex(void);

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
znerror(char *err) {
  zn_output_func errout = dsp_get_err_writer(dsp);
  errout("ERROR: %s\n", err);
}

void _process_statement()  {
  char* err = stmt_process(stmt);
  if (err) znerror(err);
}

zn_result
parse_line(zn_statement *s, zn_dispatcher *d, const char *text) { 

  stmt = s;
  dsp  = d;

  zn_scan_string(text);
  int result = znparse();
  znlex_destroy();
  if (result) {
    return zn_failure;
  }
  return zn_success;
}
