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
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/*enum file_type
{
	READ_FILE,
	WRITE_FILE
};

struct file_node {
	char *name;
	enum file_type type;
	file_node_t next;
	file_node_t prev;
};*/



void
execute_simple_command (command_t c)
{
  pid_t child = fork();
  if (child == 0) {
    int fd_in;
    int fd_out;
    if (c->input != NULL) {
      // cmd < file
      if ((fd_in = open(c->input, O_RDONLY, 0666)) == -1)
        error(1, 0, "cannot open input file!");
      if (dup2(fd_in, STDIN_FILENO) == -1)
        error(1, 0, "cannot do input redirect");
      //close(fd_in);
    }
    if (c->output != NULL) {
      // cmd > file
      //puts(c->output);
      if ((fd_out = open(c->output, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR)) == -1)
        error(1, 0, "cannot open output file!");
      if (dup2(fd_out, STDOUT_FILENO) == -1)
        error(1, 0, "cannot do output redirect");
    }
    // handle execution
    execvp(c->u.word[0], c->u.word); // one function that executes command
    error(1, 0, "can't execute command!");
  }

  else if (child > 0) {
    int status;
    // wait for the child process
    if ( (waitpid(child, &status, 0)) == -1)
      error(1, 0, "Child process error");
    // harvest the execution status
    c->status = status;
  }

  else
    error(1, 0, "cannot create child process!");
}

void
execute_and_command (command_t c) {
  execute_command(c->u.command[0], 0);

  if (c->u.command[0]->status == 0) {
      // run the second command
      execute_command(c->u.command[1], 0);
      // set the status of the AND command
      c->status = c->u.command[1]->status;
  }
  else {
      // do not run c2
      // set the status of the AND command
      c->status = c->u.command[0]->status;
  }
}

void
execute_or_command (command_t c) {
  execute_command(c->u.command[0], 0);

  //puts("TESTOR");

  if (c->u.command[0]->status == 0) {
      c->status = c->u.command[0]->status;
  }
  else {
      execute_command(c->u.command[1], 0);
      c->status = c->u.command[1]->status;
  }
}

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

     if (!time_travel) 
     {
      switch(c->type)
      {
        case SIMPLE_COMMAND:
          execute_simple_command(c);
          break;
        case AND_COMMAND:
          execute_and_command(c);
          break;
        case OR_COMMAND:
          execute_or_command(c);
          break;
        /*case PIPE_COMMAND:
          execute_pipe_command(c);
          break;
        case SEQUENCE_COMMAND:
          execute_sequence_command(c);
          break;
        case SUBSHELL_COMMAND:
          execute_subshell_command(c);
          break;*/
        default:
          error(1, 0, "Incorrect Command Type");
      }
     }

     //filler for compilation
     command_t a = c;
     int b = time_travel;
     //error (1, 0, "command execution not yet implemented");
}
