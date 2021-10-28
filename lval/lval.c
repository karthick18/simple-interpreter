/* Lval implementation routines */

/* This ones dynamic and is used by all scopes.
   lval install will take care of freeing up memory */
#include <error.h>
#include <fun.h>
#include <lval.h>
#include <data.h>
#include <scope.h>
#include <stdlib.h>
#include <string.h>

struct lval *lg_ptr = (struct lval *) NULL;

struct lval *
lval_node ()
{

  struct lval *ptr;

  ptr = (struct lval *) malloc (sizeof (struct lval));

  if (!ptr)
    {

      SET_ERROR (MEM_ERROR);

    }

  return ptr;			//return an lval node

}

void
lval_initialise ()
{

  lg_ptr = lval_node ();	//get the node
  lval_nodes_initialise ();

}

void
lval_nodes_initialise ()
{

  lg_ptr->head = (struct lval *) NULL;
  lg_ptr->tail = (struct lval *) NULL;
  lg_ptr->left = (struct lval *) NULL;
  lg_ptr->right = (struct lval *) NULL;

}

void
lval_free ()
{

  /* Remove the lval nodes. Should be called by lval_install after installation into the scope */

  struct lval *traverse;
  struct lval *temp;

  traverse = lg_ptr->head;
  while (traverse)
    {
      temp = traverse->right;
      free ((void *) traverse);
      traverse = temp;
    }

  lval_nodes_initialise ();	//reinitialise the nodes for the next usage

}

/* Final call to destroy: Cleans up the lg_ptr */

void
lval_destroy ()
{

  lval_free ();
  if (lg_ptr)
    free ((void *) lg_ptr);

}

/* Lval Load routine: Loads up the key and value into the lval structure: */

/* The  loading  makes a copy of the data before loading into the structure:

   This prevents from one or more lvals sharing a same data pointer */


void
lval_load (char *key, struct data *value)
{

  struct data *ptr = copy_data (value, 1);	//make a copy of the data

  struct variable *var = var_node ();	//get a variable node

  var->key = (char *) strdup (key);

  if (!var->key)
    {

      SET_ERROR (MEM_ERROR);

    }

  var->value = ptr;

  //deinstall the old data value as the copy is already made

  deinstall_data (value);

  //Now set up the links in the lval

  if (!lg_ptr->head)
    {

      lg_ptr->head = lval_node ();	//head node
      lg_ptr->head->var = var;
      lg_ptr->head->left = (struct lval *) NULL;
      lg_ptr->head->right = (struct lval *) NULL;

      lg_ptr->tail = lg_ptr->head;	//set up the tail

    }

  else
    {				//go to the right of tail

      lg_ptr->tail->right = lval_node ();
      lg_ptr->tail->right->var = var;
      lg_ptr->tail->right->right = (struct lval *) NULL;
      lg_ptr->tail->right->left = lg_ptr->tail;
      lg_ptr->tail = lg_ptr->tail->right;

    }


  return;

}


/* Lval Install: This one installs into the Lvalue:

Calls the scope methods: Cleans up after install:

*/

void
lval_install (unsigned long lookup_stop, int scope)
{

  struct lval *traverse;

  for (traverse = lg_ptr->tail; traverse; traverse = traverse->left)
    {

#ifdef DEBUG
      err (0, "Installing in scope:%d\n", scope);
#endif

      install_scopes (traverse->var, lookup_stop, scope);

    }
  //Now clean up the lval nodes

  lval_free ();

}
