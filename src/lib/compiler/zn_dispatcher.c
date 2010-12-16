#include "zn_dispatcher.h"

struct _dispatcher {
  write_f write;
};

zn_dispatcher* dispatcher_new() {
  zn_dispatcher *dsp = malloc(sizeof(zn_dispatcher));
  memset(dsp, 0, sizeof(zn_dispatcher));
  
  return dsp;
}

void dispatcher_reg_write(zn_dispatcher *dsp, write_f writer) {
  dsp->write = writer;
}
