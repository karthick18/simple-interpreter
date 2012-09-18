/* The scope header:

  The scope header attempts to solve the problem of scoping by having functions registered for lookups and installs for each scope.
  In this way,the block parser,can get away with the dependencies of scopes,during lookups and installs */

#ifndef _SCOPE_H
#define _SCOPE_H 1

#define SCOPES 2  //stack and global:
#include <data.h>
#include <stack.h>
#include <utils.h>

#define _ST_SCOPE 0
#define _GL_SCOPE 1

struct scope {

  struct scope_operations *scope_ptr;

};

/* The declaration of the next structure is similar to the data types operation structure. On start up each,scope will register its functions to be called by the parser. The functions are 2 as of now. More can be added in future.

 */

struct scope_operations {

  /* Install a variable into a scope given the extent of scope_lookups*/

  void (*scope_install)(struct variable *var,unsigned long lookup_stop);

  /* Look up the variables in the scopes. The lookups will be called from the scope start to the outer scopes */

  struct data * (*scope_lookup) (char *key,unsigned long lookup_stop);

};

/* End of the scope operations structure */

/* Declaration of scope dependent variables and routines */

extern struct scope scope[]; //a scope instance

extern void initialise_scopes(void); //initialise the scope

extern void register_stack_scope(void);

extern void register_global_scope(void);

extern void register_all_scopes(void); //register all the scope methods

extern void register_scope(int scope,struct scope_operations *ptr);

extern struct data *lookup_scopes (char *key,unsigned long lookup_stop,int scope_start);

extern void install_scopes(struct variable *var,unsigned long lookup_stop,int scope);

#endif
 
