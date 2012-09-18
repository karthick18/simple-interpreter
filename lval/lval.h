/* Lval structure */

#ifndef _LVAL_H
#define _LVAL_H 1

#include <stack.h>

#include <data.h>

#include <scope.h>

#include <error.h>

#include <fun.h>

#include <arg.h>

#include <utils.h>

struct lval {

  struct lval *head;
  struct lval *tail;
  struct lval *left;
  struct lval *right;
  struct variable *var;
  
};

extern struct lval *lg_ptr; 

extern struct lval * lval_node();

extern void lval_initialise();
extern void lval_free();
extern void lval_destroy();

extern void lval_load(char *key,struct data *value);

extern void lval_install(unsigned long lookup_scope,int scope); //push the lvalue into the stack

extern void lval_nodes_initialise();

#endif
