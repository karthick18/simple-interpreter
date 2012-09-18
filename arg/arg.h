/* A funtion argument structure */

#ifndef _ARG_H
#define _ARG_H 1

#include <stdio.h>
#include <data.h>
#include <stack.h>
#include <error.h>
#include <utils.h>

struct argument {

  struct argument *head;
  struct argument *tail;
  struct argument *left;
  struct argument *right;

  struct variable *var; 

};

extern void arg_install(struct argument *ptr,char *key,struct data *value);

extern void arg_initialise(struct argument *ptr); //set up head,left,right,left

extern void arg_free(struct argument *ptr);

extern void arg_stack_push(struct argument *ptr); //push into stack

extern void arg_destroy(struct argument *ptr);

extern struct argument *arg_node();

extern void arg_start(struct argument *ptr);

extern void arg_end(struct argument *ptr);

extern struct data *arg_next(struct argument *ptr,int ele_type);

#endif
