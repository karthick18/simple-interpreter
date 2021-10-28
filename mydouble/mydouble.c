/*The float object handling routines */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <data.h>

#include <myint.h>

#include <mystring.h>

#include <myfloat.h>

#include <mydouble.h>

#include <error.h>

#include <token.h>

extern Error err_type;

struct data *
dbl_not_op (struct data *a)
{

  struct data *ptr = (struct data *) NULL;

  if (a->ele_type != _DBL)
    {

      SET_ERROR (BAD_OPERAND);

    }

  ptr = MALLOC_DATA;
  if (!ptr)
    {
      SET_ERROR (MEM_ERROR);
    }

  ptr->ele_type = _DBL;
  ptr->DBL_PTR = INSTALL_DOUBLE (!a->DBL_VALUE);

  return ptr;

}


struct data *
dbl_add_op (struct data *a, struct data *b)
{

  /* A wrapper for int_operate */

  return dbl_operate (a, b, PLUS);

}


struct data *
dbl_sub_op (struct data *a, struct data *b)
{

  return dbl_operate (a, b, MINUS);

}

struct data *
dbl_mul_op (struct data *a, struct data *b)
{

  return dbl_operate (a, b, MULTIPLY);

}

struct data *
dbl_div_op (struct data *a, struct data *b)
{

  return dbl_operate (a, b, DIVIDE);
}

struct data *
dbl_compare_op (struct data *a, struct data *b, int op)
{

  return dbl_operate (a, b, op);

}

static struct object_operations operations = {

  dbl_not_op,
  NULL,
  dbl_add_op,
  dbl_sub_op,
  dbl_mul_op,
  dbl_div_op,
  dbl_compare_op,
/* Null terminated */

};


struct data *
dbl_operate (struct data *a, struct data *b, int op)
{

  struct data *ptr = (struct data *) NULL;

  struct data_operations *dptr;

  double x = 0, y = 0;

  if (a->ele_type == _INT)

    x = (double) a->INT_VALUE;

  else if (a->ele_type == _FLT)

    x = (double) a->FLT_VALUE;

  else

    x = a->DBL_VALUE;


  if (b->ele_type == _INT)

    y = (double) b->INT_VALUE;

  else if (b->ele_type == _FLT)

    y = (double) b->FLT_VALUE;

  else if (b->ele_type == _DBL)

    y = b->DBL_VALUE;


  switch (b->ele_type)
    {

    case _STR:

      CHECK_DPTR (dptr, _STR);

      switch (op)
	{

	case PLUS:
	  if (dptr->op_ptr->add_op)
	    ptr = dptr->op_ptr->add_op (a, b);

	  else
	    {

	      SET_ERROR (OPERATION_NOT_SUPPORTED);

	    }

	  break;

	case MULTIPLY:

	  if (dptr->op_ptr->mul_op)

	    ptr = dptr->op_ptr->mul_op (a, b);

	  else
	    {

	      SET_ERROR (OPERATION_NOT_SUPPORTED);

	    }

	  break;

	default:

	  SET_ERROR (BAD_OPERATOR);

	}

      break;

    case _FLT:

    case _INT:

    case _DBL:

      ptr = MALLOC_DATA;

      if (!ptr)
	{

	  SET_ERROR (MEM_ERROR);

	}
      ptr->ele_type = _DBL;

      switch (op)
	{

	case PLUS:

	  /* add the two data and get a new data */

	  ptr->DBL_PTR = INSTALL_DOUBLE (x + y);

	  break;

	case MINUS:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x - y);

	  break;

	case MULTIPLY:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x * y);

	  break;

	case DIVIDE:

	  if (!y)
	    {

	      SET_ERROR (DIV_BY_ZERO);

	    }

	  ptr->DBL_PTR = INSTALL_DOUBLE (x / y);

	  break;

	case GT:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x > y);

	  break;

	case LT:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x < y);

	  break;

	case GTE:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x >= y);

	  break;

	case LTE:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x <= y);

	  break;

	case EE:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x == y);

	  break;

	case NE:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x != y);

	  break;

	case LOGICAL_AND:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x && y);
	  break;

	case LOGICAL_OR:

	  ptr->DBL_PTR = INSTALL_DOUBLE (x || y);

	  break;

	default:

	  ptr->DBL_PTR = (struct dbl_type *) NULL;
	  deinstall_data (ptr);

	  SET_ERROR (BAD_OPERATOR);

	}

      break;

    default:

      SET_ERROR (UNKNOWN_DATATYPE);

    }				//end outer switch

  return ptr;

}				//end routine  


struct dbl_type *
install_double (double value)
{

  struct dbl_type *d;

  d = MALLOC_DBL;

  if (!d)

    {

      SET_ERROR (MEM_ERROR);

    }

  d->value = value;

  d->dbl_ptr = &operations;	//set up the operation pointer

  return d;

}


struct data *
deinstall_double (struct data *ptr)
{


  if (ptr)
    {

      if (ptr->ele_type == _DBL)
	{

	  if (ptr->DBL_PTR)

	    free ((void *) ptr->DBL_PTR);

	}

    }

  return ptr;

}




int
register_double (void)
{

  /* Register the data type */

  return register_data (&operations, _DBL);

}
