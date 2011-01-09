#ifndef ZN_RUNTIME
#define ZN_RUNTIME

#include "zn/zn_dispatcher.h"
#include "zn/defines.h"

typedef struct _runtime zn_runtime;

zn_runtime*
initialize_runtime(zn_output_func errout, zn_output_func stdout);

zn_result
runtime_exec_line(char* line);

#endif
