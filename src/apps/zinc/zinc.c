
#include "zinc/shell.h"

#include <unistd.h>
extern char **environ;

int main(int argc, char **argv) 
{
  (void) argc;
  (void) argv;

  run_shell(environ);
  return 0;

}
