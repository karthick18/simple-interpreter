/* A Rewrite of the Local Stack Handler.
  Use the same technique as the C compiler.
  Function Prologue:

   This should push the old stack frame pointer into the stack and then make the current stack frame pointer as the stack pointer.
   This will enable one to handle recursion.

   The  lookup for the local variables within the context of a function ,should start from the stack and end up at the stack frame pointer.

   The variable structure should contain: 1) Key,2) A pointer to its value.(which can be of any type.Remember that a Pointer holds machine addresses.The type of data in that address has to be type casted accordingly.

 */

#ifndef _STACK_H

#define _STACK_H 1 

#include <data.h>

#define KB  1024

#define POINTER_LEN 0x4 //the length of the pointer is 32 bits for 4 byte machines

/* Set up the stack top for a 8kb stack : 2048 stack nodes for a 8 KB stack,each holding 4 byte addresses.*/

#define STACK_TOP (8 * KB)/(POINTER_LEN) 

#define STACK_SAFETY 0x8 //set up the stack safety,stop within 8 nodes.

#define INIT_SC    (sc = STACK_TOP) //initialise the stack counter

#define IS_STACK_OVERFLOW  ( sc < STACK_SAFETY ) //check for stack overflow

#define IS_STACK_UNDERFLOW  ( sc > STACK_TOP) // check for stack underflow

/*A stack pointer is traced through a stack counter-global variable.*/

#define STACK_PTR    ( &stack[sc] )

#define STACK_DECREMENT  ( --sc )  //decrement a stack:push

#define STACK_INCREMENT  (++sc)  //increment a stack:  pop
						

/* The variable Node structure*/

struct variable {

  char *key; //the key for the variable

  struct data *value; //the value for the variable which can be of any type

};

/* The stack structure */

struct stack {

  struct variable *var;

}; 



/* The extern function and variable declarations for stack handling.*/

extern struct stack stack[];

extern int sc ; // the stack counter which has to be initialised to STACK_TOP

extern struct stack *stack_frame; //the stack frame pointer for each function.

extern void stack_push(struct variable *ptr,unsigned long lookup_stop); //push the variable structure into the stack

extern struct variable *stack_pop(void); //pop from a stack

/*Lookup a stack.Should start from the current stack pointer and proceed till the stack frame pointer*/

extern struct stack *stack_trace(char *key,unsigned long lookup_stop);

extern struct data *stack_lookup(char *key,unsigned long lookup_stop); 

extern void stack_initialise(void);

extern void stack_free(struct variable *var);

extern struct variable *var_node();

extern void stack_destroy(unsigned long extent);

#endif  /* end of stack.h*/


 
