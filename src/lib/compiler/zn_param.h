#ifndef ZN_PARAMETER
#define ZN_PARAMETER

typedef struct _param zn_param;

zn_param* param_new(void);

/* TODO: Add types. */

void*     param_get(zn_param* p);
void      param_set(zn_param* p, void* val);
void      param_destroy(void* p);

#endif
