
#include "runtime.h"

#include <stdlib.h>
#include <string.h>

#include "zinc/zn_statement.h"

char* _rt_write(zn_statement *stmt);

struct _runtime {
  zn_dispatcher *dsp;
  zn_output_func out;
};

zinc_runtime *runtime;

zinc_runtime*
initialize_runtime(zn_output_func errout, zn_output_func stdout) {
  runtime = malloc(sizeof(runtime));
  memset(runtime, 0, sizeof(runtime));

  runtime->dsp = dispatcher_new();
  runtime->out = stdout;

  dsp_reg_err_writer(runtime->dsp, errout);
  dsp_reg_write(runtime->dsp, &_rt_write);

  return runtime;
}

char* 
_rt_write(zn_statement *stmt) {
  
  zn_param *p = stmt_next_param(stmt);
  const char* msg = (const char*)param_get(p);
  runtime->out(msg);

  return NULL;
}
