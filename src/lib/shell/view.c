
#include "view.h"

#include <locale.h>
#include <stdbool.h>

#include "ncurses.h"

#include "shell_kernel.h"

typedef struct _view {
  WINDOW *output;
  int output_max_y;
  int input_line;
  bool use_color;
} view_screen;

typedef enum {
  separator_k = 1,
  command_k   = 2,
  error_k     = 3
} colors;

view_screen _vs;

/*
** Private routines, declarations
**
*/
int _handle_keystroke(void);

/*
** Public API, Implementation
**
*/

void initialize_view() {
  setlocale(LC_ALL, "");
  initscr();
  raw(); 
  keypad(stdscr, TRUE);
  echo();
  
  int y, x;

  /* Screen size. */
  getmaxyx(stdscr, y, x);

  _vs.output_max_y = y-3;
  _vs.input_line   = y-1;
  _vs.output = newwin(y-3,x-1,0,0);

  idlok(_vs.output, true);
  scrollok(_vs.output, true);

  wrefresh(_vs.output);
  
  if (has_colors()) {
    _vs.use_color = true;
    start_color();
    init_pair(separator_k, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(error_k, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(separator_k));
    mvprintw(_vs.input_line-1,0, "----------------------------------------"
	     "----------------------------------------");
    attroff(COLOR_PAIR(1));
  } else {
    _vs.use_color = false;
    mvprintw(_vs.input_line-1,0, "----------------------------------------"
	     "----------------------------------------");
  }

}

void display_prompt() {
  mvprintw(_vs.input_line, 0, "$                                       "
	    "                                        ");
  move(_vs.input_line, 2);
}
 
size_t fetch_line(char* buffer, size_t max) {

  size_t pos = 0;
  size_t endpoint = max - 1; /* Leave room for \0 */

  while (pos < endpoint) {
    int next_char = _handle_keystroke();
    buffer[pos] = next_char;
    if (next_char == '\n') {      
      int x=0, y=0; 
      buffer[pos+1] = '\0';
      getsyx(y, x);
      move(y+1,1); 
      if (_vs.use_color) 
	wattron(_vs.output, COLOR_PAIR(command_k));
      wprintw(_vs.output, "$ ");
      wprintw(_vs.output, buffer);
      if (_vs.use_color)
	wattroff(_vs.output, COLOR_PAIR(command_k));
      return pos;
    }
    pos++;
  }
  display_err("Woah, that's a big line. Too big for us!");
  return 0;
}

void printline(const char* output) {
  wprintw(_vs.output, "%s\n", output);
  wrefresh(_vs.output);
}

void display_text(const char* format, ...) {
  va_list ap;
  va_start(ap, format);

  vw_printw(_vs.output, format, ap);
  wrefresh(_vs.output);

  va_end(ap);
}

void display_err(const char* format, ...) {
  va_list ap;
  va_start(ap, format);

  if (_vs.use_color) 
    wattron(_vs.output, COLOR_PAIR(error_k));
  vw_printw(_vs.output, format, ap);
  if (_vs.use_color) 
    wattroff(_vs.output, COLOR_PAIR(error_k));
  wrefresh(_vs.output);

  va_end(ap);
}

/*
** Private routines, implemenation
**
*/

int _handle_keystroke() {
  int input = getch();
  switch(input) {
  case KEY_F(4):
    exit_shell(0);
    break;
  }
  return input;
}
