
#include "view.h"

#include <stdio.h>
#include <termios.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>

/* Local declarations */
int _process_char(char, char*);

void _add_char(char, unsigned int, char*);
void _reset_esc_seq(bool*, unsigned short*, unsigned long*);


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
  char c = 0;

  _process_char(0, NULL);

  do {
    c = getchar();
    i += _process_char(c, buffer);
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
  va_end(ap);
}  

void   display_err(const char* format, ...) {
  va_list ap;
  va_start(ap, format);

  vfprintf(stderr, format, ap);
  
  va_end(ap);
}

int
_process_char(char c, char* buffer) {

  static unsigned int i = 0;
  static bool esc_seq = false;
  static unsigned long keycode = 0;
  static unsigned short pos = 0;
  unsigned int oldpos = i;

  if (c == 0) {
    i = 0;
    esc_seq = false;
    keycode = 0;
    pos = 0;
  } else {

    /* This is an unsustainable approach. */
    switch (c) {
    case 27: /* ESC */
      esc_seq = true;
      pos = 1;
      break;
    case '[':
      if (esc_seq && pos == 1) {
	keycode = '[' << 7;
	pos++;
      } else {
	_reset_esc_seq(&esc_seq, &pos, &keycode);
	_add_char(c, i++, buffer);
      }
      break;
    case 'A':
      if (esc_seq && pos == 2) {
	printf("[UP ARROW]");
	fflush(stdin);
      } else {
	_add_char(c, i++, buffer);
      }
      _reset_esc_seq(&esc_seq, &pos, &keycode);
      break;
    case 'D':
      if (esc_seq && pos == 2) {
	printf("[LEFT ARROW]");
	fflush(stdin);
      } else {
	_add_char(c, i++, buffer);
      }
      _reset_esc_seq(&esc_seq, &pos, &keycode);
      break;
    case 'C':
      if (esc_seq && pos == 2) {
	printf("[RIGHT ARROW]");
	fflush(stdin);
      } else {
	_add_char(c, i++, buffer);
      }
      _reset_esc_seq(&esc_seq, &pos, &keycode);
      break;
    default:
      _reset_esc_seq(&esc_seq, &pos, &keycode);
      _add_char(c, i++, buffer);
    }
    
  }

  return i - oldpos;
}

void
_add_char(char c, unsigned int pos, char* buffer) {
  buffer[pos] = c;
  putc(c, stdout);
  fflush(stdout);
}

void 
_reset_esc_seq(bool *esc_seq, unsigned short *pos, unsigned long *keycode) {
  *esc_seq = false;
  *pos = 0;
  *keycode = 0;
}
