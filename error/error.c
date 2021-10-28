#include<stdio.h>
#include<error.h>
#include<token.h>
#include<utils.h>
char *err_values[] = {
  "Everything Okay",
  "Missing Semi Colon in Expression",
  "Missing Double Quotes in Expression",
  "Missing Single Quotes in Expression",
  "Invalid String Constant",
  "Syntax Error",
  "Extra Right Brace",
  "Extra Left Brace",
  "Stack OverFlow",
  "Stack Underflow",
  "Unknown Datatype",
  "Bad Operand",
  "Division by Zero",
  "Memory Allocation Error",
  "Data Type Overflow",
  "Operation Not Supported",
  "Bad Operator",
  "Bad Numeric Constant",
  "Invalid Expression",
  "Variable Not Found",
  "Bad Function Token",
  "Bad Function Argument",
  "Function Argument Mismatch",
  "Scope Register Error",
  "Scope Installation Routine Entry Missing",
  "Scope Lookup Routine Entry Missing",
  "ScopeType Unknown",
  "Function Name Not Found",
  "Function Declaration within a Function",
  "Return Not Allowed",
  "Error in if loop",
  "Else without if",
  "Break OutSide Loop",
  "Continue Outside Loop",
  "Do Without While",
  "Null Data Object",
  "For Loop Syntax Error",
  "Print Format Mismatch",
  "Print Syntax Error",
};

Error err_type = OK;

/* Count the line number at which the error occurred:
   This ones needed as its difficult to keep track of lines,
   because of the jumps on loops,and function calls */

int
count_lines (void)
{

  int lines = 1;

  extern char *program, *reference;
  char *start = program;

  while (start < reference)
    {
      if (*start == '\n')
	++lines;
      ++start;
    }

  if (current_token == NEWLINE)
    --lines;

  return lines;

}

void
error ()
{

  fprintf (stderr, "%s: Line Number %d\n", err_values[err_type],
	   count_lines ());

  main_free ();

}
