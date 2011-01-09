
#include "environment.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "view.h"

/* TODO: add a hashtable for fast lookup. */

#define MAX_ENV_SETTINGS 256

char** zn_env = NULL;

/* Private methods */
char* _get_entry(const char* name, int* pos);
int   _put_entry(char* name, char* val);

/* Public mothod implementations */

int
zn_create_env(char** initenv)
{
  int c = 0;
  if (zn_env == NULL) {
    zn_env = calloc(MAX_ENV_SETTINGS, sizeof(char*));
  }
  while(initenv[c] && c < MAX_ENV_SETTINGS) {
    zn_env[c] = malloc(strlen(initenv[c]) + 1);
    strcpy(zn_env[c], initenv[c]);
    c++;
  }
  if (c < MAX_ENV_SETTINGS) {
    zn_env[c] = NULL;
  }
  return c;
}

const char*
zn_getenv(const char* name)
{
  assert(name != NULL);
  int pos = 0;
  char* entry = _get_entry(name, &pos);
  while (*entry++ != '=');
  return entry;
}

int
zn_putenv(const char* string) 
{
  char* key = strdup(string);
  char* val = key;

  while (*val != '=') {
    assert(*val != '\0');
    val++;
  }

  *val = '\0';
  val++;

  int retval = zn_setenv(key, val, 1);
  
  free(key);
  return retval;
  
}

int
zn_setenv(const char* name, const char* val, int overwrite)
{
  if (!overwrite && zn_getenv(name)) {
    return 1;
  }

  size_t length = strlen(name) + strlen(val) + 1 + 1; /* =, \0 */
  char* entry = malloc(length);
  snprintf(entry, length, "%s=%s", name, val);

  int   pos;
  char* old = _get_entry(name, &pos);
  if (old) {
    free(old);
    zn_env[pos] = entry;
  } else if (pos < MAX_ENV_SETTINGS - 2) {
    zn_env[pos++] = entry;
    zn_env[pos] = NULL;
  } else {
    display_err("No more room in environment.\n");
    return 1;
  }

  return 0;
}

int
zn_unsetenv(const char* name)
{
  /* Implementation TBD */
  (void)name;
  return 0;
}

char* _get_entry(const char* name, int *pos) {
  *pos = 0;
  int len = strlen(name);
  while(zn_env[*pos] != NULL) {
    if (strncmp(name, zn_env[*pos], len) == 0 
	&& zn_env[*pos][len] == '=') {
      return zn_env[*pos];
    }
    (*pos)++;
  }
  return NULL;
}
