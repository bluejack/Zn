/*----------------------------------------------------------------------------*\
   
   Zn begins, as all things must, with main.

   Today, there is only a shell. I have begun with a shell rather than any of
   the more fundamental components because I want to begin with support for a
   truly interactive experience. The first, operational baby system will come
   with the full tiered infrastructure including a parser, grammar, compiler,
   runtime interpreter and "view" of that interpreter. Debugging the heart of
   the language itself will begin with all the tools that any developer would
   want at any time.

   Ultimately, this main() will support compilation, file interpretation, and
   the shell mode, with minimal usage expected to be:

   zn           <-- shell
   zn <file>    <-- interpreter
   zn -c <file> <-- compiler
  
   But for now, it is only a shell.

   Further reading:

   * http://github.com/bluejack/zn/blob/master/src/lib/shell/shell.h

\*----------------------------------------------------------------------------*/

#include "zn/shell.h"

/*
** Used to pull in the system environment.
*/
#include <unistd.h>
extern char **environ;

int main(int argc, char **argv) 
{
  (void) argc;
  (void) argv;

  run_shell(environ); 
  return 0;
}

/*----------------------------------------------------------------------------*\
   There are two mistakes one can make along the road to truth: Not going all 
   the way, and not starting.                             --Gautama Siddharta
\*----------------------------------------------------------------------------*/


