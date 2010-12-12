#ifndef ZINC_ENVIRONMENT
#define ZINC_ENVIRONMENT

extern char** zinc_env;

int
zinc_create_env(char** initenv);

const char*
zinc_getenv(const char* name);

int
zinc_putenv(const char* string);

int
zinc_setenv(const char* name, const char* val, int overwrite);

int
zinc_unsetenv(const char* name);

#endif
