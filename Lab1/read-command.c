// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h" 

#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
//////////////////////////////////////////////////////////////////////
//Global variables
////////////////////////////////////////////////////////////////////////

int linecount;
int (*get_byte) (void *);
void *get_byte_argument;

typedef struct cmd_node *cmd_node_t;
typedef struct command_node *command_node_t;

struct command_node
{
  command_t theCommand;
  command_node_t next;
  command_node_t prev;
};

struct command_stream
{
  command_node_t* commands;
};

//////////////////////////////////////////////////////////////////////

void
syntax_error()
{
  //make sure to erase allocated memory before exiting
  error(1, 0, "Syntax Error: Line %d", linecount);
}

bool is_valid(char a)
{
  // Tests for valid characters in a string that are not numbers/letters/operators
  
  switch(a)
  {
    case '!': 
      return true;
    case '%': 
      return true;
    case ',': 
      return true;
    case '-': 
      return true;
    case '+': 
      return true;
    case '.': 
      return true;
    case '/': 
      return true;
    case ':': 
      return true;
    case '@': 
      return true;
    case '^': 
      return true;
    case '_': 
      return true;        
    case '#': 
      return true;
  }
  return false;
}

bool is_special(char a)
{
  // Checks if a character is an operator
  switch (a)
  {
   case '|': 
      return true;
   case '&': 
      return true;
   case '(': 
      return true;
   case ')': 
      return true;
   case '<': 
      return true;
   case '>': 
      return true;
   case ';': 
      return true;
  }
  return false;
}

//Check to make sure statement line is valid
//Then formats/prepares strings to be turned into commands
void validationAndFormat() {

    char currChar = '\0';
    char prevChar = '\0';
    //char* commandString; //return string
    char commandString[512] = "";
    printf("%s \n", commandString);
    bool operator = false; // Tests is last two characters are '&&' or '||'
    bool openParen = false; // Tests if there is currently an open set of parentheses
    int parenCount = 0; // number of currently open parentheses

    //puts("Test1");

    currChar = get_byte(get_byte_argument);

    while(!feof(get_byte_argument)) {
    

        //prevChar = currChar;
        //currChar = get_byte(get_byte_argument);

      //putchar(currChar);

        if (currChar == EOF || currChar == '\0')
          break;

        //puts("Test2");


        //Test if currChar is an illegal character
        if (!isalnum(currChar) && !is_special(currChar) && !isspace(currChar) && !is_valid(currChar))
            syntax_error();

        // Test invalid uses of semicolon
        if (currChar == ';') {
            if (prevChar == ';' || prevChar == '\n' || prevChar == '\0')
                syntax_error();
        }

        

        //putchar(currChar);
        //puts("Test3");

        

        // Ignore everything in comments
        if (currChar == '#') {

            while (currChar != '\n' && currChar != '\0' && !feof(get_byte_argument))
                currChar = get_byte(get_byte_argument);

            if (currChar == '\0') {
              puts(commandString);
               //return commandString;
              return ;
            }

            prevChar = '\n';
            currChar = get_byte(get_byte_argument);
            continue;            
        }


        //putchar(currChar);
        //puts("Test4");

        if (currChar == ' ') {
           if (!feof(get_byte_argument)) {
               //puts("SEG1");
                prevChar = currChar;
                currChar = get_byte(get_byte_argument);
                //puts("SEG2");
            }
            else
              break;

            if ((isalnum(commandString[strlen(commandString)-1]) || is_valid(commandString[strlen(commandString)-1])) &&
                (isalnum(currChar) || is_valid(currChar)) ) {
                size_t cur_len = strlen(commandString);
                if (cur_len < 510) {
                    commandString[cur_len] = ' ';
                    continue;
                }
            }
            else {

              continue;
            }
        }

        if (currChar != '\n') {


            //putchar(currChar);
            //puts("Test5");

            // Check if operators come after newline
            if (prevChar == '\n')
              if ( is_special(currChar) && (currChar != '(' || currChar != ')') )
                syntax_error();

            // Check for misuse of &/| symbols
            if (currChar == '&' || currChar == '|') {

                if (!isalnum(commandString[strlen(commandString)-1])) {
                    if (operator == true)
                        syntax_error();

                    if (is_special(commandString[strlen(commandString)-1]) && commandString[strlen(commandString)-1] != currChar)
                        syntax_error();
                }
            }

            //putchar(currChar);
            //puts("Test6");

            // Make sure a letter follows &&/||
            if (isalnum(currChar) && operator == true)
                operator == false;

            // Keep track of parentheses
            if (currChar == '(') {
                openParen = true;
                parenCount++;
            }
            else if (currChar == ')') {
                parenCount--;
                if (parenCount == 0)
                    openParen = false;
            }
        }


        //putchar(currChar);
        //puts("Test7");



        if (currChar == '\n') {
            if (commandString[strlen(commandString)-1] == '&' || commandString[strlen(commandString)-1] == '|') {
                prevChar = currChar;
                currChar = get_byte(get_byte_argument);
                continue;
            }
            else if (isalnum(commandString[strlen(commandString)-1]) && openParen) {
                if (!feof(get_byte_argument)) {
                    //puts("SEG1");
                    currChar = get_byte(get_byte_argument);
                    //puts("SEG2");
                }
                else
                  break;

                if ((isalnum(currChar) || is_valid(currChar)) && openParen)  {
                    size_t cur_len = strlen(commandString);
                    if (cur_len < 510) {
                        commandString[cur_len] = ';';
                    }
                }
                else if (is_special(currChar) && openParen) {
                    size_t cur_len = strlen(commandString);
                    if (cur_len < 510) {
                        commandString[cur_len] = currChar;
                        continue;
                    }
                }
                else if (!openParen && !feof(get_byte_argument))
                     syntax_error();
            }

            if (prevChar == '<' || prevChar == '>')
                syntax_error();

            if ((isalnum(commandString[strlen(commandString)-1]) || is_valid(commandString[strlen(commandString)-1])) && !openParen)
            {
                puts(commandString);
                return;
            }
        } 


     




        if (isalnum(currChar) || is_valid(currChar) || is_special(currChar)) {
            size_t cur_len = strlen(commandString);
            if (cur_len < 510) {
              //putchar(currChar);
                commandString[cur_len] = currChar;
                printf("%s \n", commandString);
            }
        }

        //putchar(currChar);
        //puts("Test8"); 
        prevChar = currChar;

        if (!feof(get_byte_argument)) {
           //puts("SEG3");
           //putchar(currChar);
           currChar = get_byte(get_byte_argument);
          //puts("SEG4");
        }
        else
          break;

         //currChar = get_byte(get_byte_argument);


        //putchar(currChar);
        //puts("Test8"); 
    }



    //puts("Test9");

    if (parenCount != 0)
         syntax_error();

   //puts("Test10");

    puts(commandString);
    //return commandString;


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
        linecount++;
      ch = get_byte(get_byte_argument);
    }
    ungetc(ch, get_byte_argument);
  }
}


//Grab what type of command buffer is and return to create it
/*enum command_type
grabType(char *word_buf)
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
        else if(de==EOF)
          syntax_error();
        else
          ungetc(de, get_byte_argument);
        break;




      case '\n': line_count++;
      case ';':
      case EOF:
        return SIMPLE_COMMAND;        
    }
    strcat(word_buf, ch);
  }
  return SEQUENCE_COMMAND;
}*/


command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /*linecount = 1;
  char word_buf[512] ="";
  get_byte = get_next_byte;
  get_byte_argument = get_next_byte_argument;
  //create command stream
  command_stream_t new_stream = checked_malloc(sizeof(struct command_stream));
  command_node_t node = checked_malloc(sizeof(struct command_node));
  command_node_t temp_node = node;

  if(!feof(get_byte_argument))
  {
    remove_whitespace();
    
    //check if its empty
    strcpy(buffer,(get_byte(get_byte_argument)));
    if(buffer[0]==EOF)
    {
      free(new_stream);
      free(node);
      return NULL;
    }
    ungetc(buffer[0],get_byte_argument);
    buffer[0]='\0';
    enum command_type cmdType = grabType(buffer);

    while(1)
    {
  	  //Initialize node with command
     
    }
  }
  return stream;*/



  
  get_byte = get_next_byte;
  get_byte_argument = get_next_byte_argument;
  while(!feof(get_next_byte_argument)) {
    validationAndFormat();
  }

    command_stream_t new_stream = checked_malloc(sizeof(struct command_stream));
    return new_stream;

}

command_t
read_command_stream (command_stream_t s)
{
  if(s==NULL)
    return NULL;
  if(*(s->commands) != NULL)
  {

    
  }
  return 0;
}
