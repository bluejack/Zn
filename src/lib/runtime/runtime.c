
#include "runtime.h"

#include <stdlib.h>

struct _runtime {
  char* stub_ptr;
};

zinc_runtime*
initialize_runtime() {
  zinc_runtime* runtime = malloc(sizeof(runtime));
  return runtime;
}
