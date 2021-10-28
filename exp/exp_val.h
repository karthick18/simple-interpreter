/* The Expression Parser value stack handler: */
#ifndef _EXP_VAL_H
#define _EXP_VAL_H 1

#include <stdio.h>
#include <data.h>

#define ISVAL_EMPTY (vg_ptr->head == (struct value_stack*)NULL)

struct value_stack
{
  struct value_stack *head;
  struct value_stack *tail;
  struct value_stack *next;
  struct data *val;
  int freed;
  struct value_stack_operation *val_ptr;
};

struct value_stack_operation
{

  void (*val_push) (struct value_stack *, struct data *, int);

  int (*val_pop) (struct value_stack *, struct data **);

};

extern void val_push (struct value_stack *vg_ptr, struct data *ptr,
		      int freed);

extern int val_pop (struct value_stack *vg_ptr, struct data **ptr);

extern struct value_stack *val_node (void);

extern struct value_stack *val_initialise (void);

extern void val_free (struct value_stack *vg_ptr);

extern void val_destroy (struct value_stack *vg_ptr);

#endif
