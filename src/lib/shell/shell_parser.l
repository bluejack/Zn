ALPHA     [A-Za-z]
DIGIT     [0-9]
ALPHANUM  [A-Za-z0-9]
WORDCHAR  [A-Za-z0-9_]
WORDSTART [A-Za-z_]
TERMSTR   [^ \t\n;|'"=\003]

%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void shell_error(char *);

#include "shell_parser.tab.h"
#define YY_NO_INPUT
%}

%option nounput

%%

=                         return ASSIGN;
[;\n ]*[;\n\003]          return EOS;
exit                      return EXIT;
env                       return ENV;
:[^\n\003]*               {
                            shell_lval.text = strdup(shell_text+1);
                            return ZINCLINE;
                          }
{TERMSTR}+                {
                            shell_lval.text = strdup(shell_text);
                            return TERM;
                          }
\"[^"\n\003]*["\n\003]    {
                            shell_lval.text = strdup(shell_text+1);
                            if (shell_lval.text[shell_leng-2] != '"')
                              shell_error("Improperly terminated string.");
                            else
                              shell_lval.text[shell_leng-2] = 0;
                            return TERM;
                          }
'[^'\n\003]*['\n\003]     {
                            shell_lval.text = strdup(shell_text+1);
                            if (shell_lval.text[shell_leng-2] != '\'')
                              shell_error("Improperly terminated string.");
                            else
                              shell_lval.text[shell_leng-2] = 0;
                            return TERM;
                          }

%%

int shell_wrap(void) {
  return 1;
}
