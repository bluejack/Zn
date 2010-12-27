#ifndef ZINC_SHELL
#define ZINC_SHELL
/*----------------------------------------------------------------------------*\
                                 The Zinc Shell
  
   The zinc shell begins with the philosophy that an interactive interpretter
   is more convenient and useful if it also has access to the file system. By
   access, of course, we mean that it can do all the usual things that people
   use a modern shell for. The zinc shell has no ambition to compete with zsh
   or bash, but only to ensure that a developer working in zinc can navigate,
   explore, and interact with the file system as well as with all the runtime
   resources. 

   The shell has three basic operations:

     1. Execute filesystem shell operations. These are not pass-throughs to a
        parent shell; the zinc shell *can* be used as a full unix shell.

     2. Execute global zinc code. Any statement entered has global scope, and
        all variables declared with persist for the life of the session. Each
        statement is executed as it is entered.

     3. Enter scoped or block level code. Opening a block of any sort whether
        it be a loop, an object, or a task will convert to line editing mode.
        The user will be prompted for additional lines of zinc code until the
        block is complete. (There will be controls to exit this mode and also
        to return to it. TBD!)

   There's not much interface here: when the shell is running, it is the root
   of the whole application.

\*----------------------------------------------------------------------------*/

void run_shell(char **environ);

/*----------------------------------------------------------------------------*\
   When the music changes, so does the dance.               --African Proverb
\*----------------------------------------------------------------------------*/

#endif
