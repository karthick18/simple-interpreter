/* global hash routines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack.h>
#include <error.h>
#include <data.h>
#include <scope.h>
#include <hash.h>
#include<global.h>

struct global *global_hash_table[HASH_SIZE];

static struct scope_operations global_operations = {

  global_push,
  global_lookup,
};

struct global *
global_node ()
{

  struct global *ptr;

  ptr = (struct global *) malloc (sizeof (struct global));

  if (!ptr)
    {

      SET_ERROR (MEM_ERROR);

    }

  return ptr;

}



/* The push and lookup routines for global */

void
global_push (struct variable *var, unsigned long lookup_stop)
{

  unsigned int hashval;
  struct global *trace;

  trace = global_trace (var->key, lookup_stop);
  if (!trace)
    {

      //its a new value: install the new one and create a hash linkage 

      trace = global_node ();

      hashval = make_hash ((unsigned char *) var->key);

      trace->next = global_hash_table[hashval];

      global_hash_table[hashval] = trace;

      trace->var = var;		//push the variable into the stack

    }

  else
    {				//overwrite the data pointer

      deinstall_data (trace->var->value);

      trace->var->value = var->value;

      free ((void *) var->key);

      free ((void *) var);

    }

}


struct global *
global_trace (char *key, unsigned long lookup_stop)
{

  unsigned int hashval;
  struct global *traverse;

  hashval = make_hash ((unsigned char *) key);
  for (traverse = global_hash_table[hashval]; traverse;
       traverse = traverse->next)
    if (!strcmp (traverse->var->key, key))
      return traverse;

  return (struct global *) NULL;

}

struct data *
global_lookup (char *key, unsigned long lookup_stop)
{

  unsigned int hashval;

  struct global *traverse;

  hashval = make_hash ((unsigned char *) key);

  for (traverse = global_hash_table[hashval]; traverse;
       traverse = traverse->next)

    if (!strcmp (traverse->var->key, key))

      return traverse->var->value;

  return (struct data *) NULL;

}

/* Remove the global nodes*/

void
global_free ()
{
  int i;
  struct global *traverse;

  for (i = 0; i < HASH_SIZE; i++)
    {

      for (traverse = global_hash_table[i]; traverse;
	   traverse = traverse->next)
	{

	  free ((void *) traverse->var->key);
	  deinstall_data (traverse->var->value);

	  free ((void *) traverse->var);

	}

    }

}

/* Free up the global scope memory*/

void
global_destroy ()
{
  int i;
  struct global *traverse;
  struct global *temp;

  global_free ();
  for (i = 0; i < HASH_SIZE; i++)
    {

      temp = global_hash_table[i];

      while (temp)
	{

	  traverse = temp->next;

	  free ((void *) temp);

	  temp = traverse;

	}

    }

}


/* Register the global scope */

void
register_global_scope ()
{

  register_scope (_GL_SCOPE, &global_operations);

}
