/* Expression value stack routines */

#include<stdio.h>
#include<data.h>
#include<stdlib.h>
#include<string.h>
#include<error.h>
#include<exp_val.h>

extern Error err_type;

static struct value_stack_operation val_operations = {
  val_push,
  val_pop,
};


struct value_stack *
val_node (void)
{

  struct value_stack *ptr;

  ptr = (struct value_stack *) malloc (sizeof (struct value_stack));

  if (!ptr)
    {

      SET_ERROR (MEM_ERROR);

    }

  return ptr;

}

struct value_stack *
val_initialise (void)
{

  struct value_stack *vg_ptr;

  vg_ptr = val_node ();

  vg_ptr->head = (struct value_stack *) NULL;
  vg_ptr->tail = (struct value_stack *) NULL;
  vg_ptr->val_ptr = &val_operations;
  return vg_ptr;
}

/* Push the data pointer into the value stack */

void
val_push (struct value_stack *vg_ptr, struct data *d, int freed)
{

  if (ISVAL_EMPTY)
    {

      vg_ptr->head = val_node ();
      vg_ptr->head->val = d;
      vg_ptr->head->freed = freed;

      vg_ptr->tail = vg_ptr->head;
      vg_ptr->tail->next = (struct value_stack *) NULL;

    }

  else
    {

      vg_ptr->tail->next = val_node ();	//assign it to the next of the tail

      vg_ptr->tail->next->val = d;

      vg_ptr->tail->next->freed = freed;

      vg_ptr->tail->next->next = (struct value_stack *) NULL;

      vg_ptr->tail = vg_ptr->tail->next;


    }

}

/* Pop from the value stack. */

int
val_pop (struct value_stack *vg_ptr, struct data **ptr)
{

  struct value_stack *traverse;

  struct value_stack *prev = (struct value_stack *) NULL;

  int freed;

  if (ISVAL_EMPTY)
    {

      SET_ERROR (STACK_UNDERFLOW);

    }

  for (traverse = vg_ptr->head; traverse != vg_ptr->tail;
       prev = traverse, traverse = traverse->next);

  *ptr = vg_ptr->tail->val;

  freed = vg_ptr->tail->freed;

  free ((void *) vg_ptr->tail);

  if (!prev)

    {

      vg_ptr->head = (struct value_stack *) NULL;
      vg_ptr->tail = (struct value_stack *) NULL;

    }

  else
    {

      prev->next = (struct value_stack *) NULL;

      vg_ptr->tail = prev;

    }

  return freed;

}


/* Free up all the nodes */

void
val_free (struct value_stack *vg_ptr)
{

  struct value_stack *traverse;

  struct value_stack *temp;

  traverse = vg_ptr->head;

  while (traverse)
    {

      temp = traverse->next;
      if (traverse->freed)
	deinstall_data (traverse->val);
      free ((void *) traverse);
      traverse = temp;

    }

  vg_ptr->head = (struct value_stack *) NULL;
  vg_ptr->tail = (struct value_stack *) NULL;

}

void
val_destroy (struct value_stack *vg_ptr)
{

  if (vg_ptr)
    {

      val_free (vg_ptr);

      free ((void *) vg_ptr);
    }

}
