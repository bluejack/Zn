#include "zn_hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void _inc_key(char*, int);
void free_obj(void*);

void free_obj(void* string) {
  free((char*)string);
}

void _inc_key(char* key, int pos) {
  
  key[pos]++;
  if (key[pos] > 'z') {
    key[pos] = 'a';
    if (pos > 0) {
      _inc_key(key, pos - 1);
    } else {
      key[0] = 'a';
    }
  }
}

int main(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  zn_hash *hash = hash_new(NULL);

  char key[4] = "aaa";
  int i = 0;

  while (i++ < 25000) {
    hash_put(hash, key, strdup(key));
    _inc_key(key, 2);
  }
  
  char* out = (char*)hash_get(hash, "efg");
  if (strcmp(out, "efg") != 0) {
    printf("FAIL! expected 'efg' but got %s\n", out);
    return 1;
  }
  
  hash_destroy(hash);

  printf("Hash Table:     [OK]\n");
}

