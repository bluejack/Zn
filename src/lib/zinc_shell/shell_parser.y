
%union {
  char *text;
};

%token EOS EXIT ENV ASSIGN
%token <text> TERM

%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <stdbool.h>
  #include "zinc/shell.h"
  #include "shell_parser.h"
  #include "command.h"
  #include "command_sequence.h"
  #include "keywords.h"
  #include "view.h"

  extern FILE* yyin;
  extern int   yylineno;
  extern int   yy_scan_string(const char *);
  extern void  yylex_destroy();

  void yyerror(char*);
  int  yylex(void);
  command_seq *cmd_seq;
  command     *current;

  void _append_command(void);
%}

%%

program:
       program statement { _append_command(); }
     | statement         { _append_command(); }
     | EOS               { /* empty statement */ }
     ;

statement:
       termlist EOS      { command_set_handler(current, &zinc_shell_exec); }
     | TERM ASSIGN TERM EOS { 
                           command_add_arg(current, $1);
	                   command_add_arg(current, $3);
	                   command_set_handler(current, &zinc_shell_assign);
                         }
     | EXIT EOS          { command_set_handler(current, &zinc_shell_exit); }
     | ENV EOS           { command_set_handler(current, &zinc_shell_env);  }
     ;

termlist:
       termlist TERM     {
                           command_add_arg(current, $2);
                         }
     | TERM              {
                           command_add_arg(current, $1);
                         }
     ;
%%

void _append_command(void) {
  command_seq_add(cmd_seq, current);
  current = command_new();
}

void yyerror(char *err) {
  display_err("ERROR: %s\n", err);
}

command_seq*
parse_command(const char* input) {
  /* Remove any prior command sequence or else initialize the sequence */
  if (cmd_seq == NULL) {
    cmd_seq = command_seq_new();
  } else {
    command_seq_clear(cmd_seq);
  }
  current = command_new();

  yy_scan_string(input);
  int result = yyparse();
  yylex_destroy();
  if (result) {
    return NULL;
  }
  command_seq_reset(cmd_seq);
  return cmd_seq;
}
