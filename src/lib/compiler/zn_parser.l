ALPHA     [A-Za-z]
DIGIT     [0-9]
ALPHANUM  [A-Za-z0-9]
WORDCHAR  [A-Za-z0-9_]
WORDSTART [A-Za-z_]

%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void znerror(char *);

#include "zn_parser.tab.h"
#define YY_NO_INPUT
%}

%option nounput

%%
write                     return WRITE;
\<:                       return LEFT_PUSH;
[ \t]*\n                  return EOS;
\"[^"\n]*["\n]            {
                            znlval.text = strdup(zntext+1);
                            if (znlval.text[znleng-2] != '"')
                              znerror("Improperly terminated string.");
                            else
                              znlval.text[znleng-2] = 0;
                            return TERM;
                          }
'[^'\n]*['\n]             {
                            znlval.text = strdup(zntext+1);
                            if (znlval.text[znleng-2] != '\'')
                              znerror("Improperly terminated string.");
                            else
                              znlval.text[znleng-2] = 0;
                            return TERM;
                          }

%%

int znwrap(void) {
  return 1;
}
