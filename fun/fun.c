#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<hash.h>
#include<error.h>
#include<utils.h>
#include<token.h>
#include<fun.h>
#include<data.h>
#include<exp.h>
#include<arg.h>
#include<keywords.h>

struct fun *fun_hash_table[HASH_SIZE]; //declare a function hash_table

/*Return a function node*/

struct fun *getnode_fun(void){
  
  return  (struct fun *)malloc(sizeof(struct fun) );

}

/*Lookup a function by name */

struct fun *lookup_fun(char *name){

  struct fun *traverse;

  for(traverse=fun_hash_table[make_hash((unsigned char *)name) ]; traverse;traverse=traverse->next) 

    if(! strcmp(traverse->name,name) ) return traverse;

  return (struct fun *) NULL;

}


struct fun *install_fun(char *name,unsigned long start,unsigned long end ,int num){

  struct fun *node;

   if (! (node = lookup_fun(name) ) ) {

    /*install a new fun node*/

      if ( ! (node = getnode_fun() ) ) {

      err(0,"Fun: Error in allocating a Function Node.Returning...");

      return (struct fun *) NULL;

    }


      {

	unsigned int hashval;

        hashval = make_hash((unsigned char*)name);

        node->next = fun_hash_table[hashval];
  
        fun_hash_table[hashval] = node;

        node->name = (char*)strdup(name);

	node->start = start;

	node->end = end;

        node->args = num;

        node->arg_ptr = (struct argument *) NULL; //initialise the argument pointer
      }
  }

  else {

    node->start = start;

    node->end = end;

    node->args = num;

  }

  return node;

}


void display_fun(struct fun *ptr){

  char *s,*p;
  
  int nbytes;

  err(0,"Function name:%s\nNumber of Arguments:%d\nFunction start address:%#x\nFunction End Address:%#x:",ptr->name,ptr->args,ptr->start,ptr->end);

  nbytes = ptr->end - ptr->start;

  s=malloc(nbytes+1); 
 
  p = s;

  while(nbytes-- > 0 ) {

    *p ++ = *( (char*)ptr->start );
 
    ptr->start++;

  }

  *p = 0;

  #ifdef DEBUG
  err(0,"\nDisplaying the Function from start to end:\n:%s:",s);
  #endif
  free((void*) s);
  
}


void fun_free() {
  int i;
  struct fun *traverse;

  for(i=0; i < HASH_SIZE; i++) 
    {

      for(traverse= fun_hash_table[i]; traverse;traverse=traverse->next) {

        free((void*)traverse->name);

	arg_destroy(traverse->arg_ptr);
        

      }

    }

}


/* Free up the global scope memory*/

void fun_destroy() {
  int i;
  struct fun *traverse;
  struct fun *temp;

  fun_free();
  for(i=0;i<HASH_SIZE;i++) {

    temp= fun_hash_table[i];

    while(temp) {

      traverse=temp->next;
   
      free((void*)temp);

      temp = traverse;

    }

  }

}


/*Traverse the function table*/

void traverse_fun(void){

  int i;

  struct fun *traverse;

  for(i=0;i<HASH_SIZE;i++){

    for(traverse=fun_hash_table[i];traverse; traverse=traverse->next) 

      display_fun(traverse);


  }


}    


/* The function handler that will be called by the expression parser */

/* The function handler should parse the arguments,by calling the expression parser,and then push the arguments into the stack in reverse order.
 It should do the argument checking,and token validation,before doing an argument push. After it pushes the argument into the stack,it should push the return address into the stack,and call the function prologue.
 The function prologue should mark the keys with the data pointers,
 and then push the old stack frame. Make the stack pointer the new stack frame,and call the block_parser with a stack scope as the argument,the lookup_stop,the parse stop,as the argument. 
 After the return from the block_parser,the function prologue,should do pop up the stack frame ,pop up the return address from the stack,before returning.
 The function_handler should then clean up the arguments pushed into the stack before calling the expression parser which returns back to the block_parser */

struct data *function_handler(struct fun *fun_ptr,unsigned long lookup_stop,int scope) {

  struct data *ptr;
  struct stack *esp;

  int args= 0;
  char *old_ptr; //to save the reference first
  unsigned long ret_addr; //to save the return address  
  unsigned long stop_addr,start_addr;//to save the stop address
  old_ptr = reference; 

  get_stop_addr(&stop_addr); //load the stop address
 
  ret_addr = stop_addr+1; //set the return address


 #ifdef DEBUG
 err(0,"Ret addr=%c:%#x\n",*c,ret_addr);
 #endif
 reference = old_ptr; //reset the reference

 get_token();

 if(current_token != RP) {

    reference = old_ptr; 
 
   fun_ptr->arg_ptr = arg_node();

   arg_initialise(fun_ptr->arg_ptr);

   //push the arguments into the stack.
 
   start_addr = (unsigned long)reference;

   while(start_addr < stop_addr) {
      
     arg_install(fun_ptr->arg_ptr,(char*)NULL,exp_evaluate(stop_addr,lookup_stop,scope) ); //stop address and the scope id

     ++args;

     start_addr = (unsigned long)reference;

   }

   
   if(start_addr == stop_addr) {

     SET_ERROR(BAD_FUNCTION_ARGUMENT);

   }

 }

   if(fun_ptr->args != args) {


     SET_ERROR(FUNCTION_ARGUMENT_MISMATCH);

   }

   //push the arguments into the stack
 
  #ifdef DEBUG
   err(0,"Pushing the arguments into the stack:\n");
  #endif

   if(args) {
  
    arg_stack_push(fun_ptr->arg_ptr); 

    arg_destroy(fun_ptr->arg_ptr);

   }

   #ifdef DEBUG
   err(0,"Number of arguments:%d",args);
   #endif

   fun_ptr->arg_ptr = (struct argument *)NULL;

 
 //push return address into the stack

 push_addr(ret_addr);

 //Now call the function prologue to parse the function

 ptr = function_prologue(fun_ptr); 

 //Now destroy the stack by the number of arguments pushed
   
 esp = (struct stack*)STACK_PTR  + args;  //get the stop address

 stack_destroy((unsigned long)esp); //destroy the stack
 
 return ptr; //return the data pointer 

}

/*Push Return address into the Stack */

void push_addr(unsigned long addr) {

  struct variable *var = var_node();
  var->key = (char *)NULL; 

  var->value = MALLOC_DATA; //get the data
 
  var->value->ele_type = _INT;

  var->value->INT_PTR = INSTALL_INT((int)addr);
  
  //Push into stack

  stack_push(var,(unsigned long)0);
 
  return ;

}


/* The function prologue routine:  */

/*This routine will load the keys in the stack:
  Then it will push the stack frame into the stack:
  After that it will copy the stack frame pointer as the esp,and call the block parser. On return,it will call the function epilogue which will destroy the stack upto the extent of the stack frame,and then pop up the stack frame and the return address correctly before returning the data pointer.*/

struct data *function_prologue(struct fun *fun_ptr) { 
 
  int args=0,loop_flag=OFF;
  extern char *reference;
  extern struct stack *stack_frame;
  struct data *ptr;
  unsigned long lookup_stop;

  reference = (char *)fun_ptr->start;//set up the reference to the start of the function arguments
  while(get_token() != RP) {
    if(current_token == IDENTIFIER) {
      ++args;
      load_key(tok,args); //load into the stack
    }
  }

  //Now push the stack frame pointer into the stack

  push_addr((unsigned long)stack_frame); 

  //Now copy the stack pointer to the stack frame
 
  stack_frame = STACK_PTR; 

  //Set up the lookup stop pointer

  lookup_stop = (unsigned long) ( (struct stack *)stack_frame + 3 + args ); //hit the return address in the stack

  get_token(); 
  //Now call the block parser

  ptr = block_parser(lookup_stop,_ST_SCOPE,1,1,&loop_flag); //accepts return statements

  //Now after the function is parsed,call the the function epilogue

  function_epilogue(); //free up the stack,and then pop up frame and return address

  return ptr;

}

void load_key(char *key,int n) {
 
  struct stack *esp = (struct stack *)STACK_PTR + n + 1;
 
  esp->var->key = (char*)strdup(key);

}

/* The function epilogue.Frees up the stack and sets up the stack frame and ret address */

void function_epilogue(void) {
 
  unsigned long extent;
  extern struct stack *stack_frame;
  extern char *reference;
  struct variable *var;

  extent = (unsigned long) stack_frame  ; //get the extent for destroy

  stack_destroy(extent); //destroy the contents of the stack

  //Pop up the stack frame
 
  var = stack_pop();

  stack_frame = (struct stack *) (unsigned long)( var->value->INT_VALUE) ;

  stack_free(var);

  //Pop up the return address

  var =stack_pop();

 #ifdef DEBUG
  err(0,"Ret check:%c:%#x\n",*c,ret);
 #endif

  reference  = (char *) (unsigned long) (var->value->INT_VALUE); //set up the reference

  stack_free(var); 

  return ;

}   
 
 


  
