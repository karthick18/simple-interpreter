/* The expression parser:
 The Expression parser uses a Parser table,which denotes the action to take for the token in the operator stack and the current token.
*/

#include <stdio.h>
#include <exp_val.h>
#include <exp_op.h>
#include <data.h>
#include <stack.h>
#include <token.h>
#include <error.h>
#include <exp.h>
#include <stdlib.h>
#include <scope.h>
#include<fun.h>
#include<intern.h>

char action_table[MAX_OP - 1][MAX_OP] = {

  {SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL,
   SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL,
   SPECIAL, SPECIAL, SPECIAL, SPECIAL},

  {SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL,
   SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL, SPECIAL,
   SPECIAL, SPECIAL, SPECIAL, SPECIAL},
  /* * */
  {SPECIAL, SPECIAL, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, SHIFT, REDUCE, REDUCE},
  /* / */
  {SPECIAL, SPECIAL, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, SHIFT, REDUCE, REDUCE},
  /* + */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, SHIFT, REDUCE, REDUCE},
  /* - */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, SHIFT, REDUCE, REDUCE},
  /* >> */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, SHIFT, REDUCE, REDUCE},
  /* << */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, SHIFT, REDUCE, REDUCE},
  /* > */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, E4, E4, E4, E4,
   E4, E4, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE, SHIFT, REDUCE, REDUCE},
  /* < */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, E4, E4, E4, E4,
   E4, E4, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE, SHIFT, REDUCE, REDUCE},
  /* >= */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, E4, E4, E4, E4,
   E4, E4, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE, SHIFT, REDUCE, REDUCE},
  /* <= */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, E4, E4, E4, E4,
   E4, E4, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE, SHIFT, REDUCE, REDUCE},
  /* == */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, E4, E4, E4, E4,
   E4, E4, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE, SHIFT, REDUCE, REDUCE},
  /* != */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, E4, E4, E4, E4,
   E4, E4, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE, SHIFT, REDUCE, REDUCE},
  /* & */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   SHIFT, REDUCE, REDUCE},
  /* ^ */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   SHIFT, REDUCE, REDUCE},
  /* | */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, SHIFT, SHIFT, REDUCE, REDUCE, REDUCE,
   SHIFT, REDUCE, REDUCE},
  /* && */
  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT,
   SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, REDUCE, REDUCE, SHIFT,
   REDUCE, REDUCE},

  /* || */

  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT,
   SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, REDUCE, SHIFT,
   REDUCE, REDUCE},

  /* lp */

  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT,
   SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT,
   SHIFT, E2},

  /* rp */

  {SPECIAL, SPECIAL, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE,
   REDUCE, E3, REDUCE, REDUCE},

  /*end */

  {SPECIAL, SPECIAL, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT,
   SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, SHIFT, E1,
   EVALUATE},

};




void
exp_destroy (struct value_stack *vg_ptr, struct operator_stack *og_ptr)
{

  val_destroy (vg_ptr);
  op_destroy (og_ptr);

}



/*Return a data type pointer by loading the ele_type*/

struct data *
get_data (int tok)
{

  struct data *ptr = MALLOC_DATA;

  if (!ptr)
    {

      SET_ERROR (MEM_ERROR);
    }

  ptr->ele_type = (DataType) tok;
  return ptr;

}

/*Evaluate the expression*/

struct data *
evaluate (struct value_stack *vg_ptr, struct operator_stack *og_ptr)
{

  struct data *ptr;

  val_pop (vg_ptr, &ptr);	//pop from the value stack
  if (!ISVAL_EMPTY && !ISOP_EMPTY)
    {
      exp_destroy (vg_ptr, og_ptr);


      SET_ERROR (INVALID_EXPRESSION);

    }

  exp_destroy (vg_ptr, og_ptr);

  return ptr;
}

struct data *
handle_identifier (int sign, char *tok, int action, unsigned long lookup_stop,
		   int scope)
{


  struct data *ptr, *cp, *dp = (struct data *) NULL;
  struct fun *fun_ptr;
  char *old;
  char l[TOKEN + 1];

  old = reference;

  my_strncpy (l, tok, TOKEN);

  get_token ();

  if (current_token == LP)
    {
      fun_ptr = lookup_fun (l);
      if (!fun_ptr)
	{

	  SET_ERROR (FUNCTION_NOT_FOUND);

	}
      ptr = dp = function_handler (fun_ptr, lookup_stop, scope);

    }
  else
    {

      reference = old;

      ptr = lookup_scopes (l, lookup_stop, scope);

      if (!ptr)
	{
	  SET_ERROR (VARIABLE_NOT_FOUND);
	}

    }

  cp = copy_data (ptr, sign);

  ptr = operate_data (cp, (struct data *) NULL, action);

  deinstall_data (cp);

  if (dp)
    deinstall_data (dp);

  return ptr;

}


void
exp_error (struct value_stack *vg_ptr, struct operator_stack *og_ptr,
	   int action)
{

  extern char *token_classes[];

  fprintf (stderr, "%s\n", token_classes[action]);

  exp_destroy (vg_ptr, og_ptr);

  exit (1);

}

void
exp_shift (struct operator_stack *og_ptr, int tok)
{

  /* Push the token into the Operator stack */

  op_push (og_ptr, (char) tok);

}

void
exp_reduce (struct value_stack *vg_ptr, struct operator_stack *og_ptr, int ct)
{

  /*Reduce the expression,and push the result into the value stack */

  struct data *a;
  struct data *b;

  int f1, f2;

  int t;

  t = (int) op_pop (og_ptr);	//pop up the operator stack 

  if (t == RP)

    op_pop (og_ptr);

  else
    {

      f1 = val_pop (vg_ptr, &b);
      f2 = val_pop (vg_ptr, &a);
      val_push (vg_ptr, operate_data (a, b, t), 1);	//just call operate data irrespective of the operation to be performed. The value can be freed

      if (f1)
	{

#ifdef FREE

	  err (0, "Deinstalling data:\n");

#endif

	  deinstall_data (a);

	}

      if (f2)
	{

#ifdef FREE

	  err (0, "Deinstalling Data:\n");

#endif

	  deinstall_data (b);

	}


    }


}				//end reduce    


int
get_sign ()
{
  char *old;
  extern char *reference;
  int sign = 1;

  old = reference;
  get_token ();
  if (current_token != MINUS && current_token != PLUS)
    reference = old;
  else
    sign = (current_token == MINUS) ? -1 : 1;

  return sign;
}

/*Evaluate an expression*/
/*The expression parser code*/

struct data *
exp_evaluate (unsigned long stop_addr, unsigned long lookup_stop, int scope)
{
  int ct = SPECIAL;
  extern char tok[];
  struct data *ptr_1;
  struct data *ptr_2;
  struct fun *fun_ptr;
  struct intern *builtin;

  struct value_stack *vg_ptr;
  struct operator_stack *og_ptr;

  int sign = 1;
  char l[TOKEN + 1];
  char *old;
  char *stop_ptr = (char *) NULL;

  if (stop_addr)
    stop_ptr = (char *) stop_addr;

  vg_ptr = val_initialise ();
  og_ptr = op_initialise ();

  op_push (og_ptr, (char) END);	//push the END into the OP stack

  /* Fetch the sign at the start of the expression parser */

  sign = get_sign ();

  ct = get_token ();

  while (1)
    {


      if (err_type != OK)
	{

	  SET_ERROR (err_type);

	}

      if (ct == TERMINATOR
	  || (stop_ptr && (reference > stop_ptr || ct == COMMA)))
	ct = END;		//reset everything to end

      else if (ct == COMMA || ct == NEWLINE || !*reference)
	{

	  SET_ERROR (SYNTAX_ERROR);
	}


      if (ct == LOGICAL_NOT || ct == ONES_COMPLEMENT)
	{

	  int action = ct;

	  int sign = 1;		//keep track of signed values

	  int tries = 0;

	repeat:

	  ct = get_token ();

	  if (ct == IDENTIFIER || ct == CONSTANT_INT || ct == CONSTANT_FLOAT)
	    {

	      if (ct == IDENTIFIER)

		{


		  ptr_2 =
		    handle_identifier (sign, tok, action, lookup_stop, scope);

		}

	      else if (ct == CONSTANT_INT)
		{

		  ptr_1 = install_data (get_data ((int) _INT), tok);

		  ptr_1->INT_VALUE = sign * ptr_1->INT_VALUE;

		  ptr_2 = operate_data (ptr_1, (struct data *) NULL, action);

		  free ((void *) ptr_1);

		}

	      else
		{

		  ptr_1 = install_data (get_data ((int) _FLT), tok);

		  ptr_1->FLT_VALUE = sign * ptr_1->FLT_VALUE;

		  ptr_2 = operate_data (ptr_1, (struct data *) NULL, action);

		  free ((void *) ptr_1);

		}

	      val_push (vg_ptr, ptr_2, 1);	//push into the value stack

	      ct = get_token ();

	      continue;

	    }


	  else
	    {

	      if (!tries && (ct == PLUS || ct == MINUS))
		{

		  sign = (ct == MINUS) ? -1 : 1;

		  goto repeat;
		}

	      else
		{

		  SET_ERROR (INVALID_EXPRESSION);
		}

	    }


	}

      else
	{

	  DataType dt;
	  int action;

	  switch (ct)
	    {

	    case IDENTIFIER:
	      my_strncpy (l, tok, TOKEN + 1);
	      old = reference;

	      get_token ();

	      if (current_token == LP)
		{		//function call

		  builtin = lookup_intern (l);

		  if (builtin)
		    {

		      --reference;	//push back

		      ptr_1 = builtin->intern_ptr (lookup_stop, scope, 1);

		    }
		  else
		    {

		      fun_ptr = lookup_fun (l);	//lookup the function 

		      if (!fun_ptr)
			{

			  err (0, "Function Name:%s not found:\n", l);
			  SET_ERROR (FUNCTION_NOT_FOUND);

			}

		      ptr_1 = function_handler (fun_ptr, lookup_stop, scope);

		    }

		  if (!ptr_1)
		    return evaluate (vg_ptr, og_ptr);

		  if (sign < 0)
		    {

		      ptr_2 = copy_data (ptr_1, sign);

		      deinstall_data (ptr_1);

		      ptr_1 = ptr_2;

		      sign = 1;

		    }

		  val_push (vg_ptr, ptr_1, 1);

		  ct = get_token ();	//get the next token

#ifdef DEBUG
		  err (0, "Function:The value of ct=%d\n", ct);
		  token_info ();
#endif

		}

	      else
		{		//reread the token

#ifdef DEBUG
		  err (0, "Looking up %s in %d scope:\n", l, scope);
#endif

		  ptr_1 = lookup_scopes (l, lookup_stop, scope);

		  if (!ptr_1)
		    {

		      SET_ERROR (VARIABLE_NOT_FOUND);

		    }

#ifdef DEBUG
		  show_data (ptr_1);
#endif

		  val_push (vg_ptr, copy_data (ptr_1, sign), 1);	//push into the stack

		  if (sign < 0)
		    sign = 1;

		  reference = old;

		  ct = get_token ();

		}

	      break;

	    case CONSTANT_INT:
	    case CONSTANT_FLOAT:
	    case CONSTANT_STRING:

	      if (ct == CONSTANT_INT)
		dt = _INT;
	      else if (ct == CONSTANT_FLOAT)
		dt = _FLT;
	      else
		dt = _STR;

	      ptr_1 = install_data (get_data ((int) dt), tok);

	      if (sign < 0)
		{

		  ptr_2 = copy_data (ptr_1, sign);

		  sign = 1;

		  deinstall_data (ptr_1);

		  ptr_1 = ptr_2;

		}

#ifdef DEBUG
	      err (0, "Pushing %s into the value stack:\n", tok);
#endif
	      val_push (vg_ptr, ptr_1, 1);	//push into the stack

	      ct = get_token ();

	      break;

	    default:		//an operator

	      if (ct >= MAX_OP - 1)
		{

		  SET_ERROR (INVALID_EXPRESSION);

		  return evaluate (vg_ptr, og_ptr);

		}


	      action = (int) action_table[(int) og_ptr->tail->op][ct];

	      switch (action)
		{

		case SHIFT:

#ifdef DEBUG
		  err (0, "Shift operation:\n");
#endif
		  exp_shift (og_ptr, ct);

		  ct = get_token ();

		  break;

		case REDUCE:
#ifdef DEBUG
		  err (0, "Reduce Operation:\n");
#endif
		  exp_reduce (vg_ptr, og_ptr, ct);

		  break;

		case EVALUATE:
#ifdef DEBUG
		  err (0, "Evaluation:\n");
#endif
		  return evaluate (vg_ptr, og_ptr);

		default:

		  exp_error (vg_ptr, og_ptr, action);

		}

	    }


	}

    }

}
