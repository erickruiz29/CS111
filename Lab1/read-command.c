// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h" 

#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
//////////////////////////////////////////////////////////////////////
//Global variables
////////////////////////////////////////////////////////////////////////

linecount = 1;


//////////////////////////////////////////////////////////////////////
//struct cmd_node
//{
//  command cmd;
//  cmd_node next;
//};

//struct command_stream
//{
//  cmd_node commands;
//};
//////////////////////////////////////////////////////////////////////

//Check to make sure 
bool isStatementValid(char* statement);

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  //create command stream
  command_stream_t new_stream = checked_malloc(sizeof(struct command_stream));
  
  char ch = get_next_byte(get_next_byte_argument);
  
  while(ch != EOF)
  {
	  //Check errors
    //....

    //Get first word
    char word_buf[1024] ="";
    while(ch != ' ' && ch != '\t' && ch != '\n')
    {
      strcat(word_buf, ch);

    }
  }
  
  return stream;
}

command_t
read_command_stream (command_stream_t s)
{
  
  return 0;
}
