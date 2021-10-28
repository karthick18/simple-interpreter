#ifndef _ERROR_H
#define _ERROR_H 1

#include<stdio.h>
#define NUMBER 3

typedef enum
{
  OK,
  MISSING_SEMI_COLON,
  MISSING_DOUBLE_QUOTES,
  MISSING_SINGLE_QUOTES,
  INVALID_CONSTANT,
  SYNTAX_ERROR,
  EXTRA_RIGHT_BRACE,
  EXTRA_LEFT_BRACE,
  STACK_OVERFLOW,
  STACK_UNDERFLOW,
  UNKNOWN_DATATYPE,
  BAD_OPERAND,
  DIV_BY_ZERO,
  MEM_ERROR,
  DATA_TYPE_OVERFLOW,
  OPERATION_NOT_SUPPORTED,
  BAD_OPERATOR,
  BAD_NUMERIC_CONSTANT,
  INVALID_EXPRESSION,
  VARIABLE_NOT_FOUND,
  BAD_FUNCTION_TOKEN,
  BAD_FUNCTION_ARGUMENT,
  FUNCTION_ARGUMENT_MISMATCH,
  SCOPE_REGISTER_ERROR,
  SCOPE_INSTALL_MISSING,
  SCOPE_LOOKUP_MISSING,
  SCOPE_UNKNOWN,
  FUNCTION_NOT_FOUND,
  FUNCTION_WITHIN_FUNCTION,
  RETURN_NOT_ALLOWED,
  IF_ERROR,
  ELSE_WITHOUT_IF,
  BREAK_OUTSIDE_LOOP,
  CONTINUE_OUTSIDE_LOOP,
  DO_WITHOUT_WHILE,
  NULL_DATA,
  FOR_SYNTAX_ERROR,
  PRINT_FORMAT_MISMATCH,
  PRINT_SYNTAX_ERROR,
} Error;

extern char *err_values[];

extern Error err_type;

extern void error (void);

extern int count_lines (void);

extern void main_free (void);

#endif
