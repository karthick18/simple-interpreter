
#include <stdio.h>
#include <error.h>
#include <exp_op.h>
#include<stdlib.h>
#include<string.h>
#include <data.h>


extern Error err_type;

struct operator_stack_operation op_operations = {

  op_push,
  op_pop,

};


struct operator_stack *op_node(){

  struct operator_stack *ptr;

  ptr = (struct operator_stack*)malloc(sizeof(struct operator_stack));

  if(! ptr) {

    SET_ERROR(MEM_ERROR);

  }

  return ptr;

}

struct operator_stack *op_initialise() {

  struct operator_stack *og_ptr;

  og_ptr = op_node();

  og_ptr->head = (struct operator_stack*)NULL;
  
  og_ptr->tail = (struct operator_stack*)NULL;
  
  og_ptr->op_ptr = &op_operations;

  return og_ptr;

}

/* Push a character into the operator stack */

void op_push(struct operator_stack *og_ptr,char op) {

  if(ISOP_EMPTY) {

    og_ptr->head = op_node();

    og_ptr->head->op = op;

    og_ptr->head->next=(struct operator_stack*)NULL;

    og_ptr->tail = og_ptr->head;

  }

  else {

    og_ptr->tail->next = op_node();

    og_ptr->tail->next->op = op;

    og_ptr->tail->next->next = (struct operator_stack*)NULL; 
    
    og_ptr->tail = og_ptr->tail->next;

  }

}

char op_pop(struct operator_stack *og_ptr) {

  struct operator_stack *traverse;
  struct operator_stack *prev= (struct operator_stack *)NULL;

  char op;

  if(ISOP_EMPTY ) {

    SET_ERROR(STACK_UNDERFLOW);

  }

  for(traverse = og_ptr->head ; traverse != og_ptr->tail ; prev=traverse,traverse=traverse->next);

  op = og_ptr->tail->op;

  free((void*)og_ptr->tail);

  if(! prev) {

    og_ptr->head = (struct operator_stack *)NULL;
    og_ptr->tail = (struct operator_stack*)NULL;

  }
 
  else {

    og_ptr->tail = prev;

    og_ptr->tail->next = (struct operator_stack*)NULL;

  }

  return op;

}

void op_free(struct operator_stack *og_ptr) {

  struct operator_stack *traverse;

  struct operator_stack *temp;

  traverse=og_ptr->head;

  while(traverse) { 

    temp = traverse->next;

    free((void*)traverse);

    traverse=temp;

  }

  og_ptr->head = (struct operator_stack*)NULL;
  og_ptr->tail = (struct operator_stack*)NULL;

}

void op_destroy(struct operator_stack *og_ptr){


  if(og_ptr) {
  
    op_free(og_ptr);

    free((void*)og_ptr);
  }
}



