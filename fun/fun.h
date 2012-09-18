#ifndef _FUN_H

#include<stdio.h>
#include<arg.h>
#include<data.h>
#include<stack.h>
#include <scope.h>
#define _FUN_H 1
#define FUN_NAME 100
#define ISFUN(name)  (! strcmp("fun",name) )
#define VALID_FUN() ( (last_token == NEWLINE) || (last_token == TERMINATOR) || (last_token == END) )

struct fun{

  char *name; //function name

  int args ; //number of arguments

  unsigned long start;

  unsigned long end; 
  
  struct argument *arg_ptr; //arguments slot
  
  struct fun *next;  //next function lookup slot

};

extern struct fun *fun_hash_table[]; //function hash table slots

extern struct fun *install_fun(char *name,unsigned long start,unsigned long end,int num);

extern struct fun *lookup_fun(char *name); 

extern struct fun *getnode_fun(void);

extern void display_fun(struct fun *ptr);

extern void traverse_fun(void);

extern struct data *function_handler(struct fun *fun_ptr,unsigned long lookup_stop,int scope);

extern struct data *function_prologue(struct fun*fun_ptr);

extern void load_key(char *key,int narg);

extern void function_epilogue(void);

extern void push_addr(unsigned long addr);

extern void fun_free(void);

extern void fun_destroy(void);

#endif

