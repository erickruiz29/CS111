// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <error.h>
#include <errno.h>
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
  if (child == 0)
  {
    int fd_in;
    int fd_out;
    if (c->input != NULL)
    {
      // cmd < file
      if ((fd_in = open(c->input, O_RDONLY, 0666)) == -1)
        error(1, errno, "cannot open input file!");
      if (dup2(fd_in, STDIN_FILENO) == -1)
        error(1, errno, "cannot do input redirect");
      close(fd_in);
    }
    if (c->output != NULL)
    {
      // cmd > file
      //puts(c->output);
      if ((fd_out = open(c->output, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR)) == -1)
        error(1, errno, "cannot open output file!");
      if (dup2(fd_out, STDOUT_FILENO) == -1)
        error(1, errno, "cannot do output redirect");
      close(fd_out);
    }
    // handle execution
    if(execvp(c->u.word[0], c->u.word) == -1) // one function that executes command
    {
      //printf("%d",errno);
      if(c->u.word[0][0] != ':') //skip this example
        error(1,errno,c->u.word[0]);
    }
    exit(c->status);
    //error(1, 0, "can't execute command!");
  }

  else if (child > 0)
  {
    int status;
    // wait for the child process
    if ( (waitpid(child, &status, 0)) == -1)
      error(1, errno, "Child process error");
    // harvest the execution status
    c->status = status;
  }

  else
    error(1, 0, "cannot create child process!");
}

void
execute_pipe_command (command_t c)
{
  int pipefd[2];
  pipe(pipefd);
  pid_t child = fork();

  //printf("Child: %d\n", child);

  //child
  if(child == 0)
  {
    close(pipefd[1]);
    int dup = dup2(pipefd[0],0);
    if(dup == -1)
      //printf("error dup==-1\n");
      error(1, errno, "file error");
    execute_command(c->u.command[1],0);
    close(pipefd[0]);
    exit(c->u.command[1]->status);
  }
  //parent
  else if(child > 0)
  {
    pid_t child2 = fork();
    if(child2 == 0)
    {
      close(pipefd[0]);
      int dup = dup2(pipefd[1],1);
      if(dup == -1)
        //printf("error dup==-1\n");
        error(1, errno, "file error");
      execute_command(c->u.command[0],0);
      close(pipefd[1]);
      exit(c->u.command[0]->status);
    }
    else if(child2 > 0)
    {
      close(pipefd[0]);
      close(pipefd[1]);
      int status;
      pid_t wait_pid = waitpid(-1, &status,0);
      if(wait_pid == child)
      {
        c->status = status;
        waitpid(child2,&status,0);
        return;
      }
      else if(wait_pid == child2)
      {
        waitpid(child,&status,0);
        c->status = status;
        return;
      }
    }
    else
      error(1,errno,"Child process error");
  }
  else
    error(1, errno, "forking error");
}

void
execute_and_command (command_t c)
{
  execute_command(c->u.command[0], 0);

  if (command_status(c->u.command[0]) == 0) {
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

  if (command_status(c->u.command[0]) == 0) {
      c->status = c->u.command[0]->status;
  }
  else {
      execute_command(c->u.command[1], 0);
      c->status = c->u.command[1]->status;
      //printf("%d\n", c->type);
  }
}

void
execute_sequence_command(command_t c) {
  execute_command(c->u.command[0], 0);
  execute_command(c->u.command[1], 0);
  c->status = c->u.command[1]->status;
}

void
execute_subshell_command(command_t c) {
  execute_command(c->u.subshell_command, 0);
  c->status = c->u.subshell_command->status;
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
     if(c == NULL)
      return;
    //printf("Enter%d\n", c->type);
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
        case PIPE_COMMAND:
          execute_pipe_command(c);
          break;
        case SEQUENCE_COMMAND:
          execute_sequence_command(c);
          break;
        case SUBSHELL_COMMAND:
          execute_subshell_command(c);
          break;
        default:
          error(1, 0, "Incorrect Command Type");
      }
     }
     //printf("Exit%d\n", c->type);
     //filler for compilation
     //command_t a = c;
     int b = time_travel;
     //error (1, 0, "command execution not yet implemented");
}
