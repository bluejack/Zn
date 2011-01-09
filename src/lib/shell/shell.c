#include <stdio.h>
#include <stdlib.h>

#include "shell.h"
#include "shell_parser.h"
#include "command_sequence.h"
#include "view.h"
#include "shell_kernel.h"

#include "zn/runtime.h"

#define LINE_BUFFER_LEN FILENAME_MAX * 4

void
run_shell(char **env_list)
{
  initialize_shell(env_list);
  initialize_runtime(&display_err, &display_text);

  char line_buffer[LINE_BUFFER_LEN];

  while(1) {

    size_t len = fetch_line(line_buffer, LINE_BUFFER_LEN);

    if (len > 0) {
      command_seq *cmd_seq = parse_command(line_buffer);
      
      while (cmd_seq && command_seq_has_next(cmd_seq))
	set_last_exit(command_seq_exec_next(cmd_seq));
    }  
  }    
}
  
