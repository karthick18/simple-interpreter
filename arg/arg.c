/*Argument routine handlers*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arg.h>
#include<error.h>
#include<stack.h>

extern Error err_type;

struct argument *arg_node() {
  struct argument *ptr;
  ptr = (struct argument *)malloc(sizeof(struct argument) );
  if (!ptr) 
    {
      SET_ERROR(MEM_ERROR);

    }

  return ptr;
}

void arg_initialise(struct argument *ptr) {

  ptr->head = (struct argument *)NULL;
  ptr->tail = (struct argument *)NULL;
  ptr->left = (struct argument *)NULL;
  ptr->right= (struct argument *)NULL;
  ptr->var  = (struct variable *)NULL;

}

/* Only install the data into the argument stack */
   
/* The variable to be installed can be or cannot be freed */

void arg_install(struct argument *ptr,char *key,struct data *value) {
  /*Install an argument and manipulate the head and tail pointers*/

  if(!ptr->head ) { //first argument to be installed

    ptr->head = arg_node(); //get an argument node
    
    ptr->head->var = var_node(); //get a variable node

    ptr->head->var->key = key;

    ptr->head->var->value = value; //set up the value of the variable

    /* Now build up the chain correctly */

    ptr->head->right = (struct argument *) NULL;

    ptr->head->left = (struct argument *) NULL; 

    ptr->tail = ptr->head ; //set up the tail pointer

  }

  else { //an argument already exists.Place this one appropriately and adjust the links

    ptr->tail->right = arg_node(); //get an argument node for setting up pointers
   
    ptr->tail->right->var = var_node();

    ptr->tail->right->var->key = key;

    ptr->tail->right->var->value = value;
    
    ptr->tail->right->left = ptr->tail; //adjust the left node of the current one

    ptr->tail->right->right = (struct argument *) NULL; 

    ptr->tail = ptr->tail->right; //readjust the tail pointer


  }

} //end of routine


/* Push the arguments into the stack */

void arg_stack_push(struct argument *ptr) { 
  /*Push the arguments into the stack */

  struct argument *traverse;

  /* Push in reverse order*/

  for(traverse=ptr->tail; traverse; traverse=traverse->left) {

   #ifdef DEBUG
    show_data(traverse->var->value);
   #endif 
    stack_push(traverse->var,(unsigned long)0);  //push the variable into the stack

  }

}

void arg_free(struct argument *ptr) {

  struct argument *traverse;

  struct argument *temp;

  traverse = ptr->head;

  while(traverse) {

    temp = traverse->right;

    free((void*)traverse);

    traverse=temp;

  }

  
}

void arg_destroy(struct argument *ptr) {

  if (ptr) {
    arg_free(ptr);
    free((void*)ptr);

  }

}

 
/*Used for Printf manipulation */  
void arg_start(struct argument *ptr) {
  ptr->tail = ptr->head;
}


struct data *arg_next(struct argument *ptr,int ele_type) {

  ptr->tail = ptr->tail->right;
 
  if(!ptr->tail) {

    SET_ERROR(NULL_DATA);
  }

  if(ele_type >= 0) {
  if(ptr->tail->var->value->ele_type != ele_type) {
    SET_ERROR(PRINT_FORMAT_MISMATCH);
  }
  }

  return ptr->tail->var->value;

}

void arg_end(struct argument *ptr) {
 
  struct argument *traverse;

  for(traverse = ptr->head;traverse;traverse=traverse->right)
    stack_free(traverse->var);

  arg_destroy(ptr);

}


