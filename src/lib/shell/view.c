
#include "view.h"

#include <stdio.h>
#include <termios.h>
#include <stdarg.h>
#include <unistd.h>

static struct termios default_settings;
     
void initialize_view(void) {

  struct termios view_settings;
  
  tcgetattr(0,&default_settings);
  
  view_settings = default_settings;
  
  /* Disable canonical mode, and set buffer size to 1 byte */
  view_settings.c_lflag &= (~ICANON);
  view_settings.c_lflag &= (~ECHO);
  view_settings.c_lflag &= (~FLUSHO);
  view_settings.c_cc[VTIME] = 0;
  view_settings.c_cc[VMIN] = 1;
  
  tcsetattr(0,TCSANOW,&view_settings);
}

void close_view(void) {
  tcsetattr(0,TCSANOW,&default_settings);
}

void display_prompt(void) {
  printf(" $ ");
  fflush(stdout);
}

size_t fetch_line(char* buffer, size_t buffer_len) {

  unsigned int i = 0;
  unsigned int max = buffer_len - 1;
  fd_set inputfds;
  unsigned int stdinfd = fileno(stdin);
  char linebuf[128];
  char linemax = 128;
  char c = 0;

  do {
    FD_ZERO(&inputfds);
    FD_SET(stdinfd,&inputfds);

    select(stdinfd+1, &inputfds, NULL, NULL, NULL);

    ssize_t found = read(stdinfd, linebuf, linemax);

    if (found > 0) {
      c = linebuf[0];
      if (found == 1) {
	buffer[i++] = c;
	putc(c, stdout);
	fflush(stdout);
      } else {
	printf("Got a block.\n");
      }
    }
  } while ( i < max && c != '\n' );
  
  buffer[i] = '\0';
  return i;
}

void   printline(const char* output) {
  printf("%s\n", output);
}

void   display_text(const char* format, ...) {
  va_list ap;
  va_start(ap, format);

  vprintf(format, ap);
  printf("\n");
  va_end(ap);
}  

void   display_err(const char* format, ...) {
  va_list ap;
  va_start(ap, format);

  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}

