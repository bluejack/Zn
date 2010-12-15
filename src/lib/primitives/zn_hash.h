#ifndef ZN_HASH
#define ZN_HASH
/*----------------------------------------------------------------------------*\
** Hash Table
**
** A very basic hash table based on the DJB string hashing algorithm.
**
**           Copyright (c) 2010-2011, Blunt Jackson
**
\*----------------------------------------------------------------------------*/
#include <stdbool.h>

#include "zn_mem.h"

typedef struct _hash zn_hash;

zn_hash* hash_new(object_destructor);
void     hash_destroy(zn_hash*);
void     hash_put(zn_hash*, const char* key, void* val);
void*    hash_get(zn_hash*, const char* key);
unsigned long hash_size(zn_hash*);
unsigned long hash_collisions(zn_hash*);
unsigned long hash_replacements(zn_hash*);
#endif
