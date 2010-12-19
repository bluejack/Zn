ALPHA     [A-Za-z]
DIGIT     [0-9]
ALPHANUM  [A-Za-z0-9]
WORDCHAR  [A-Za-z0-9_]
WORDSTART [A-Za-z_]

%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void yyerror(char *);

#include "zn_parser.tab.h"
#define YY_NO_INPUT
%}

%option nounput

%%
write                     return WRITE;
\<--                       return LEFT_PUSH;
[ \t]*\n                  return EOS;
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
