/* A sample test main routine that checks the performance of the expression parser */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <data.h>
#include <stack.h>
#include <exp.h>
#include <token.h>
#include <utils.h>
#include<error.h>
#include<scope.h>
#include <lval.h>
#include <fun.h>
#include<arg.h>
#include<global.h>
#include<keywords.h>
char *program;
char *reference;
int lines, unknown;

void parse_global_scope ();

int
main (int argc, char *argv[])
{

  program = initialise (argc, argv);
  if (!program)
    {
      err (1, "Terminating the program:\n");
    }

  reference = program;		//set up the global reference pointer for the program

  initialise_scopes ();
  register_all_scopes ();	//register all the scopes

  initialise_datatypes ();	//initialise the data types
  register_all_datatypes ();	//set up the data type handlers
  stack_initialise ();		//set up the stack

  lval_initialise ();		//initialise the lvalue handler

  function_precursor ();	//call function precursor to load functions

  reference = program;

  parse_global_scope ();

  main_free ();			//free up the memory

  exit (0);

}

void
parse_global_scope ()
{

  int loop_flag = OFF;

  extern Token current_token;

  char *tail = program + strlen (program);

  *tail++ = '\n';

  *tail++ = '}';

  *tail = '\0';

  current_token = LB;		//set up the current_token

  err (0, "Launching the Syntax Checker:\n");

  block_parser ((unsigned long) 0, _GL_SCOPE, 0, 0, &loop_flag);	//run the syntax checker

  reference = program;		//reset for the program parser

  current_token = LB;

  block_parser ((unsigned long) 0, _GL_SCOPE, 0, 1, &loop_flag);	//run the parser 

}

void
show_data (struct data *ptr)
{

  if (ptr)
    {
      switch (ptr->ele_type)
	{

	case _INT:

	  err (0, "The value of the Integer Object is %d:\n", ptr->INT_VALUE);
	  break;

	case _FLT:

	  err (0, "The value of the Float Object is %.2f:\n", ptr->FLT_VALUE);
	  break;

	case _DBL:

	  err (0, "The value of the Double Object is %.2f:\n",
	       ptr->DBL_VALUE);
	  break;


	case _STR:

	  err (0, "The value of the string object is %s\n", ptr->STR_VALUE);

	  break;

	default:

	  err (0, "Unknown Data Type:\n");

	  main_free ();

	}

    }


}

void
main_free ()
{

  extern struct stack stack[];

  unsigned long extent = (unsigned long) &stack[STACK_TOP];

  stack_destroy (extent);	//destroy the stack
  lval_destroy ();
  traverse_fun ();		//display the functions
  fun_destroy ();
  global_destroy ();

  if (program)
    free ((void *) program);

  err (1, "Freed up the memory.Wrapping up Main:\n");

}
