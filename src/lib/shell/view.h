#ifndef ZN_SHELL_VIEW
#define ZN_SHELL_VIEW

#include <stdlib.h>

void initialize_view(void);

void close_view(void);

size_t fetch_line(char* buffer, size_t buffer_len);

void   printline(const char* output);

void   display_text(const char* format, ...);

void   display_err(const char* format, ...);

#endif
