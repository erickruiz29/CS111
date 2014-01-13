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

int linecount;
int (*get_byte) (void *);
void *get_byte_argument;

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

//Check to make sure statement line is valid
bool isStatementValid(char* statement);

void
syntax_error()
{
  error(1, 0, "Syntax Error: Line %d", linecount);
}

void 
remove_whitespace()
{
  if(!feof(get_byte_argument))
  {
    char ch = get_byte(get_byte_argument);
    while(strchr("\n\t ", ch))
    {
      if(ch == '\n')
        line_count++;
      ch = get_byte(get_byte_argument);
    }
    ungetc(ch, get_byte_argument);
  }
}


//Grab what type of command buffer is and return to create it
enum command_type
grabBuffer(char *word_buf)
{
  
  while(!feof(get_byte_argument))
  {

    //make sure the comment is in the right place
    char ch = get_byte(get_byte_argument);
    char de = get_byte(get_byte_argument);
    if(de == '#' && !strchr("\n\t ", ch))
     syntax_error();
    ungetc(de, get_byte_argument);

    //the special characters that make the unique cases
    switch(ch) 
    {
      case '#':
        de = get_byte(get_byte_argument);
        //get to end of comment
        while(de != '\n')
        {
            if(de == EOF)
              return SIMPLE_COMMAND;
            de = get_byte(get_byte_argument);
        }
        return grabBuffer(word_buf);

      case '&':
        de = get_byte(get_byte_argument);
        if(de == '&')
        { 
          remove_whitespace();
          return AND_COMMAND;
        }

      case '\n': line_count++;
      case ';':
      case EOF:
        return SIMPLE_COMMAND;        
    }
    strcat(word_buf, ch);
  }
  return SEQUENCE_COMMAND;
}


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
