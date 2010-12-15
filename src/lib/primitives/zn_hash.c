
#include "zn_hash.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Hash size must be a power of 2 in order to perform modulo by bitwise and. */
#define HASH_SIZE 1024
#define HASH_MOD  1023

/* -------------------------------------------------------------------------- *\
   structures
\* -------------------------------------------------------------------------- */

typedef struct _node {
  struct _node *next;
  char* key;
  void* obj;
} hnode;

typedef struct _hash {
  hnode *table[HASH_SIZE];
  object_destructor destroy;
  unsigned long size;
  unsigned long collisions;
  unsigned long replacements;
} _hash;

/* -------------------------------------------------------------------------- *\
   private declarations
\* -------------------------------------------------------------------------- */

hnode* _make_node(const char* key, void *obj);
unsigned int _index(const char* key);

/* -------------------------------------------------------------------------- *\
   public implementations
\* -------------------------------------------------------------------------- */

zn_hash* 
hash_new(object_destructor destructor) {
  zn_hash* h = malloc(sizeof(zn_hash));
  memset(h, 0, sizeof(zn_hash));  
  h->destroy = destructor;
  return h;
}

void     
hash_destroy(zn_hash* h) {
  for (int i = 0; i < HASH_SIZE; i++) {
    hnode* hn = h->table[i];
    while(hn) {
      hnode *next = hn->next;
      free(hn->key);
      if (h->destroy != NULL) {
	h->destroy(hn->obj);
      }
      free(hn);
      hn = next;
    }
  }
  free(h);
}

void     
hash_put(zn_hash* h, const char* key, void* val) {
  int i = _index(key);
  if (h->table[i] == NULL) {    
    h->table[i] = _make_node(key, val);
  } else {
    hnode *node = h->table[i];
    hnode *next = node;;
    /* TODO: There's gotta be a more elegant loop here. */
    do {
      node = next;
      next = node->next;
      /* Replace existing node if match. (Replacement) */
      if (strcmp(node->key, key) == 0) {
	if (h->destroy != NULL) {
	  h->destroy(node->obj);
	}
	node->obj = val;
	h->replacements++;
	return;	
      }
    } while (next);
    /* Append new node to list if no match (Collision)*/
    h->collisions++;
    node->next = _make_node(key, val);
  }
  h->size++;
}

void*    
hash_get(zn_hash* h, const char* key) {
  void* ret = NULL;
  int i = _index(key);
  if (h->table[i] != NULL) {
    hnode* node = h->table[i];
    while (node) {
      if (strcmp(node->key, key) == 0) ret = node->obj;
      node = node->next;
    }
  }
  return ret;
}

unsigned long 
hash_size(zn_hash* h) {
  return h->size;
}

unsigned long
hash_collisions(zn_hash* h) {
  return h->collisions;
}

unsigned long
hash_replacements(zn_hash* h) {
  return h->replacements;
}

/* -------------------------------------------------------------------------- *\
   private implementations
\* -------------------------------------------------------------------------- */

hnode* 
_make_node(const char* key, void* val) {
  hnode* node = malloc(sizeof(hnode));
  node->key = strdup(key);
  node->obj = val;
  node->next = NULL;
  return node;
}

unsigned int _index(const char* key) {
  unsigned long hashidx = 5381;
  unsigned long keylen  = strlen(key);

  for(unsigned long pos = 0; pos < keylen; pos++) {
    hashidx = ((hashidx << 5) + hashidx) + key[pos];
  }
  return hashidx & HASH_MOD;
}
