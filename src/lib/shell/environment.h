#ifndef ZN_ENVIRONMENT
#define ZN_ENVIRONMENT

extern char** zn_env;

int
zn_create_env(char** initenv);

const char*
zn_getenv(const char* name);

int
zn_putenv(const char* string);

int
zn_setenv(const char* name, const char* val, int overwrite);

int
zn_unsetenv(const char* name);

#endif
