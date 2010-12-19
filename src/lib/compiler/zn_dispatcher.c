#include "zn_dispatcher.h"

#include <stdlib.h>
#include <string.h>

struct _dispatcher {
  zn_output_func  errout;
  zn_stmt_handler write;
};

zn_dispatcher* dispatcher_new() {
  zn_dispatcher *dsp = malloc(sizeof(zn_dispatcher));
  memset(dsp, 0, sizeof(zn_dispatcher));
  
  return dsp;
}

void dsp_reg_write(zn_dispatcher *dsp, zn_stmt_handler writer) {
  dsp->write = writer;
}

void dsp_reg_err_writer(zn_dispatcher *dsp, zn_output_func out) {
  dsp->errout = out;
}

zn_stmt_handler dsp_get_write(zn_dispatcher *dsp) {
  return dsp->write;
}

zn_output_func 
dsp_get_err_writer(zn_dispatcher *dsp) {
  return dsp->errout;
}
