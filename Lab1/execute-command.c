// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

enum file_type
{
	READ_FILE,
	WRITE_FILE
};

struct file_node {
	char *name;
	enum file_type type;
	file_node_t next;
	file_node_t prev;
};

int
command_status (command_t c)
{
  return c->status;
}

void
execute_command (command_t c, int time_travel)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */

     //filler for compilation
     command_t a = c;
     int b = time_travel;
  error (1, 0, "command execution not yet implemented");
}
