
%union {
  char *text;
};

%token LEFT_PUSH WRITE
%token <text> TERM

%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <stdbool.h>

  extern FILE* yyin;
  extern int   yylineno;
  extern int   yy_scan_string(const char *);
  extern void  yylex_destroy();

  void yyerror(char*);
  int  yylex(void);

  void _prepare_statement();
%}

%%

program:
       program statement { _prepare_statement(); }
     | statement         { _prepare_statement(); }
     | EOS               { /* empty statement */ }
     ;

statement:
       WRITE LEFT_PUSH TERM { 
     ;
%%

void 
yyerror(char *err) {
  display_err("ERROR: %s\n", err);
}

void _process_statement()  {
  /* Do something? */
}

/* TODO. don't like the circular dependency here... */
command_seq*
parse_statement(zn_parser *parser, const char* input) {
  yy_scan_string(input);
  int result = yyparse();
  yylex_destroy();
  if (result) {
    return NULL;
  }
  command_seq_reset(cmd_seq);
  return cmd_seq;
}
