
#include "view.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include <readline/readline.h>
#include <readline/history.h>
     
void initialize_view(void) {
}

void close_view(void) {
}

size_t fetch_line(char* buffer, size_t buffer_len) {

  char* line = readline("$ ");
  if (line) {
    unsigned int line_len = strlen(line);
    buffer_len--; /* Save room for a terminating null */ 
    buffer_len--; /* Save room for an END OF TEXT     */
    unsigned int len = line_len >= buffer_len ? buffer_len : line_len;
    
    strncpy(buffer, line, len);
    buffer[len++] = 3; /* End-of-Text */
    buffer[len]   = 0; /* End-of-String */

    add_history(line);

    free(line);
    return len + 1;    /* Because we added the end of text char. */
  } else {
    buffer[0] = '\0';
    return 0;
  }
}

void   printline(const char* output) {
  printf("%s\n", output);
}

void   display_text(const char* format, ...) {
  va_list ap;
  va_start(ap, format);

  vprintf(format, ap);
  va_end(ap);
}  

void   display_err(const char* format, ...) {
  va_list ap;
  va_start(ap, format);

  vfprintf(stderr, format, ap);
  
  va_end(ap);
}
