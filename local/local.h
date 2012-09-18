/* Local stack management header*/
#ifndef _LOCAL_H
#define _LOCAL_H 1

#define TOP  100 /*stack top set to 100*/

struct local {

  char *key;
  char *value; 

}local[TOP]; //define the stack for the local variable with key and value

extern struct local *esp; //the stack pointer for the local variable stack

extern int local_push(char *key,char *value);//push into the stack

extern struct local *local_lookup(char *key); //lookup the local variable stack

extern void local_destroy(unsigned int extent); //destroy the stack

extern void local_initialise(void);

#endif
