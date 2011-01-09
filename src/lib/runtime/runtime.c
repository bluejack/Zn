
#include "runtime.h"

#include <stdlib.h>
#include <string.h>

#include "zn/zn_statement.h"
#include "zn/zn_parser.h"

char* _rt_write(zn_statement *stmt);

struct _runtime {
  zn_dispatcher *dsp;
  zn_output_func out;
  zn_output_func err;
};

zn_runtime *runtime;

zn_runtime*
initialize_runtime(zn_output_func errout, zn_output_func stdout) {
  runtime = malloc(sizeof(runtime));
  memset(runtime, 0, sizeof(runtime));

  runtime->dsp = dispatcher_new();
  runtime->out = stdout;
  runtime->err = errout;

  dsp_reg_err_writer(runtime->dsp, errout);
  dsp_reg_write(runtime->dsp, &_rt_write);

  return runtime;
}

/*
 * Concern: we have the execution happening within the parser;
 * what happens when we have an open block? I think we'll need to reshuffle
 * this.
 */

zn_result
runtime_exec_line(char* line) {
  
  zn_statement *stmt = stmt_new();

  return parse_line(stmt, runtime->dsp, line);
}

char* 
_rt_write(zn_statement *stmt) {
  
  stmt_reset(stmt);
  zn_param *p = stmt_next_param(stmt);
  const char* msg = (const char*)param_get(p);
  runtime->out(msg);

  return NULL;
}

