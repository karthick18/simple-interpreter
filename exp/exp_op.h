/*The expression parser operator stack handler */

#ifndef _EXP_OP_H
#define _EXP_OP_H 1
#include<stdio.h>

#define ISOP_EMPTY (og_ptr->head == (struct operator_stack*)NULL)

struct operator_stack {

  struct operator_stack *head;
  struct operator_stack *tail;
  struct operator_stack *next;

  struct operator_stack_operation *op_ptr;
  char op; //the character to push

};

struct operator_stack_operation {

  void (*op_push) (struct operator_stack *,char op);
  char (*op_pop) (struct operator_stack *);

};


extern struct operator_stack *op_node();
extern void op_push(struct operator_stack *og_ptr,char op);
extern char op_pop(struct operator_stack *og_ptr);
extern struct operator_stack *op_initialise();
extern void op_free(struct operator_stack *og_ptr);
extern void op_destroy(struct operator_stack *og_ptr);

#endif
