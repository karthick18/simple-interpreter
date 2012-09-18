/* The definition routines for stack.c */

#include <stack.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h> 

#include <error.h>

#include<scope.h>

struct stack stack[STACK_TOP+1];

/* Initialise the scope operations */

static struct scope_operations scope_operations = {

  /* install routine */
  stack_push,
  /* Lookup routine registration */
  stack_lookup,

};

/* Initialise the globals in this file to the correct Values */

extern Error err_type;

int sc;

struct stack *stack_frame;

/* Initialise the stack*/

void stack_initialise(void) {
  struct stack *esp;
  struct stack *traverse;
  INIT_SC;
  esp = STACK_PTR;
  for(traverse=esp; traverse >= esp - STACK_TOP; traverse--) 
    traverse->var = (struct variable*)NULL;
      
stack_frame = STACK_PTR;
}

struct variable *var_node() {

  struct variable *v;
  v = (struct variable *)malloc(sizeof(struct variable)) ;

  if(!v){

    SET_ERROR(MEM_ERROR);
  }

  return v;
}

/* Definition of the Push routine for stack. */

/*Patched for stack lookups before pushes*/

void stack_push(struct variable *var,unsigned long lookup_stop) { 

  /* check for overflow before pushing*/

  struct stack *esp;

  struct stack *lookup;


   if (IS_STACK_OVERFLOW) {

      err_type = STACK_OVERFLOW;

      error(); 

   }

   /* push the value into the stack */

   esp = STACK_PTR;
      
   lookup = stack_trace(var->key,lookup_stop);

   if(! lookup) { //new install
     
     esp->var = var;

     if(esp->var->key) {
    
     #ifdef DEBUG 
        err(0,"New Install:%s\n",esp->var->key);
     #endif
      
     }


     STACK_DECREMENT;

   }

   else {

     //overwrite the old install
     deinstall_data(lookup->var->value);
     lookup->var->value = var->value;
     if(var->key) free((void*)var->key);
     free((void*)var);
 
   }

   return ;

  }

struct stack *stack_trace(char *key,unsigned long lookup_stop) {

  struct stack *traverse;
  struct stack *esp = (struct stack*)STACK_PTR + 1;
  struct stack *stop =(struct stack *)lookup_stop;
  if(!key ) return (struct stack *)NULL;

  for(traverse=esp;traverse<stop;traverse ++)

    
    if(traverse->var->key) 

      if(! strcmp(traverse->var->key,key) )

        return traverse;

    return (struct stack*)NULL;

  }
   
/* The pop routine for stack.*/

struct variable *stack_pop(void) {  

  struct stack *esp;

  STACK_INCREMENT; //increment the stack

  if(IS_STACK_UNDERFLOW) {

   err_type =STACK_UNDERFLOW;
  
   error(); 

  }

  esp = STACK_PTR;
 
  return esp->var;

 } 

/* Lookup a stack for the local variable and value*/

/*Patched for lookup stops:Search till esp < stop */

struct data *stack_lookup(char *key,unsigned long lookup_stop) { 

  struct stack *traverse;
  
  struct stack *esp = (struct stack *)STACK_PTR + 1;

  struct stack *stop = (struct stack *)lookup_stop;
 
  if(! key) return (struct data*) NULL;

  for(traverse=esp; traverse < stop ; traverse ++) 
      
    if(traverse->var->key )

    if(! strcmp(traverse->var->key,key) ) 

      return traverse->var->value;

  return (struct data*) NULL;

}

/*Destroy the stack upto the extent specified.*/

void stack_destroy(unsigned long extent) { 

  struct stack *esp = STACK_PTR; //get the stack pointer

  struct stack *to = (struct stack*)extent;

  struct variable *var;

  while(esp < to) {

    /*Pop up the stack */

    var = stack_pop();

   #ifdef DEBUG
    if(var->key) { 
      err(0,"Removing variable :%s\n",var->key);
    }

   #endif

    stack_free(var); //free up var

    esp = STACK_PTR;  //reset stack pointer

  }

}


void stack_free(struct variable *var){

  if(var ) {
    if(var->key) free((void*)var->key); 

    if(var->value) deinstall_data(var->value);

    free((void*)var);

  }

  return ;

}
    
    
 void register_stack_scope(void) { 

   register_scope(_ST_SCOPE,&scope_operations);
 }


