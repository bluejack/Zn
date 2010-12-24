ALPHA     [A-Za-z]
DIGIT     [0-9]
ALPHANUM  [A-Za-z0-9]
WORDCHAR  [A-Za-z0-9_]
WORDSTART [A-Za-z_]
TERMSTR   [^ \t\n;|'"=]

%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void yyerror(char *);

#include "shell_parser.tab.h"
#define YY_NO_INPUT
%}

%option nounput

%%

=                         return ASSIGN;
[\n;]+[ ;\n]*             return EOS;
exit                      return EXIT;
env                       return ENV;
:[^\n]*                   {
                            yylval.text = strdup(yytext+1);
                            return ZINCLINE;
                          }
{TERMSTR}+                { 
                            yylval.text = strdup(yytext);
                            return TERM;
                          }
\"[^"\n]*["\n]            {
                            yylval.text = strdup(yytext+1);
                            if (yylval.text[yyleng-2] != '"')
                              yyerror("Improperly terminated string.");
                            else
                              yylval.text[yyleng-2] = 0;
                            return TERM;
                          }
'[^'\n]*['\n]             {
                            yylval.text = strdup(yytext+1);
                            if (yylval.text[yyleng-2] != '\'')
                              yyerror("Improperly terminated string.");
                            else
                              yylval.text[yyleng-2] = 0;
                            return TERM;
                          }

%%

int yywrap(void) {
  return 1;
}
