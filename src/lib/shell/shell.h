#ifndef ZN_SHELL
#define ZN_SHELL
/*----------------------------------------------------------------------------*\
                                  The Zn Shell
  
   The Zn shell begins with the philosophy that an interactive console should
   be as close to a fully functional shell as possible. While the Zn shell is
   not intended to compete with zsh, bash or the like, it does hope that when
   a developer working in Zn needs navigate and interact with the file system
   as well as with all the runtime resources, there is no inconvenience.

   The shell has three basic operations:

     1. Execute filesystem shell operations. These are not pass-throughs to a
        parent shell; the Zn shell *can* be used as a full unix shell.

     2. Execute global and scoped Zn code. Valid Zn statements entered in the
        console will have immediate global scope, while all objects and tasks
        have the appropriate scope declared. Zn files may be loaded directly,
        or referenced as from any Zn script. 

     3. Inspect the interactive runtime. All active and available objects and
        tasks may be inspected from the console.

   There's not much interface here: when the shell is running, it is the root
   of the whole application.

\*----------------------------------------------------------------------------*/

void run_shell(char **environ);

/*----------------------------------------------------------------------------*\
   When the music changes, so does the dance.               --African Proverb
\*----------------------------------------------------------------------------*/

#endif
