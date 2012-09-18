/*Definitions of the tokens are in this file.*/

#ifndef _TOKEN_H
#define _TOKEN_H

#include<stdio.h>
#include<stack.h>
#include<utils.h>
#include<data.h>
#define TOKEN 8 * KB //set it to the stack size

#define MAX_DECIMAL 12

#define SINGLE_QUOTES 047
#define DOUBLE_QUOTES 042
#define IS_SIGNED (last_token == EQUAL || last_token == TERMINATOR || last_token == NEWLINE || last_token == END) 

typedef enum {
  LOGICAL_NOT,
  ONES_COMPLEMENT,
  MULTIPLY,
  DIVIDE,
  PLUS,
  MINUS,
  RS,
  LS,
  GT,
  LT,
  GTE,
  LTE,
  EE,
  NE,
  BITWISE_AND,
  BITWISE_XOR,
  BITWISE_OR,
  LOGICAL_AND,
  LOGICAL_OR,
  LP,
  RP,
  END,
  EQUAL,
  MAX_OP,
  TERMINATOR,
  LB,
  RB,
  COMMA,
  DOT,
  HASH,
  BACKSLASH,
  KEYWORDS,
  IDENTIFIER,
  CONSTANT_INT,
  CONSTANT_FLOAT,
  CONSTANT_STRING,
  NEWLINE,
  UNKNOWN,
  SPECIAL,
  REDUCE,
  SHIFT,
  EVALUATE,
  E1, /* missing left parenthesis*/
  E2, /*missing right parenthesis*/
  E3, /*extra left parenthesis*/
  E4, /*illegal comparison combination*/  
}Token;


extern char  *token_classes[]; //mapping for the tokens

extern Token current_token ;

extern Token last_token;

extern char allowed[];

extern char tok[];

extern char *program;

extern char *reference;

extern int get_token();

extern int token_info(void);

extern int push_back(int);

extern void handle_sign(void);

#endif
