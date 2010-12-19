#ifndef ZINC_RUNTIME
#define ZINC_RUNTIME

#include "zinc/zn_dispatcher.h"

typedef struct _runtime zinc_runtime;

zinc_runtime*
initialize_runtime(zn_output_func errout, zn_output_func stdout);

#endif
