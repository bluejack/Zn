#ifndef ZN_SHELL_KERNEL
#define ZN_SHELL_KERNEL

/*
** Note this is a stateful API -- only one shell is expected to be running per
** application, and the state of that shell is maintained and made available
** through this api.
**
*/

void initialize_shell(char **env);

void exit_shell(int exit_val);

void set_last_exit(int exit_val);

int  get_last_exit(void);

#endif
