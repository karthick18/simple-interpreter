/* The scope implementation routines that will be called by the block parser */

#include <stdio.h>
#include <stdlib.h>
#include <stack.h>
#include <data.h>
#include <utils.h>
#include <fun.h>
#include <scope.h>
#include <error.h>

struct scope scope[SCOPES];	//declaration of the scope structure

/* Initialise the scopes */

void
initialise_scopes (void)
{

  struct scope *ptr;
  int i;

  for (i = 0; i < SCOPES; i++)
    {

      ptr = &scope[i];
      ptr->scope_ptr = (struct scope_operations *) NULL;
    }

}

/* Register all the scopes */

void
register_all_scopes (void)
{
  register_stack_scope ();
  register_global_scope ();
}


void
register_scope (int n, struct scope_operations *p)
{

  if (n >= SCOPES)
    {

      SET_ERROR (SCOPE_REGISTER_ERROR);

    }

  else
    {

      struct scope *ptr = &scope[n];

      ptr->scope_ptr = p;

    }

}

/* Install into scope */

void
install_scopes (struct variable *var, unsigned long lookup_stop, int s)
{

  struct scope *ptr = &scope[s];	//have a pointer to the scope

  if (!ptr->scope_ptr)
    {

      SET_ERROR (SCOPE_UNKNOWN);

    }

  if (!ptr->scope_ptr->scope_install)
    {

      SET_ERROR (SCOPE_INSTALL_MISSING);

    }

  ptr->scope_ptr->scope_install (var, lookup_stop);


}


/* Lookup the scope: Start from the present scope and proceed towards the outer scope */

struct data *
lookup_scopes (char *key, unsigned long lookup_stop, int start)
{

  int i;

  struct scope *ptr;
  struct data *dp;

  for (i = start; i < SCOPES; i++)
    {

      ptr = &scope[i];

      if (!ptr->scope_ptr)
	{

	  SET_ERROR (SCOPE_UNKNOWN);
	}

      if (!ptr->scope_ptr->scope_lookup)
	{
	  SET_ERROR (SCOPE_LOOKUP_MISSING);

	}

      dp = ptr->scope_ptr->scope_lookup (key, lookup_stop);
      if (dp)
	return dp;		//return the data pointer 
    }

  return (struct data *) NULL;	//variable not found

}
