#include "zn_param.h"

#include <stdlib.h>
#include <string.h>

struct _param {
  void *value;
};

zn_param* param_new() {
  zn_param* p = malloc(sizeof(zn_param));
  memset(p, 0, sizeof(zn_param));
  return p;
}

void* param_get(zn_param *p) {
  return p->value;
}

void param_set(zn_param *p, void *v) {
  p->value = v;
}

void param_destroy(void *p) {
  free(((zn_param*)p)->value);
  free(p);
}
