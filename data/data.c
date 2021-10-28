/* Implementation of data abstraction routines.

   Install data and operate data.
   Install data: Installs a data object.

   Operate data: Operates on a data object.

   Deinstall data: Deinstalls a data object.
*/

#include <stdio.h>

#include <string.h>

#include <data.h>

#include <token.h>

#include <myint.h>

#include <myfloat.h>

#include <mydouble.h>

#include <mystring.h>

#include <error.h>

#include <stdlib.h>

#include<utils.h>

extern Error err_type;

struct data_operations data_operations[DATA_TYPES];

/*Make a copy of the data object*/

struct data *
copy_data (struct data *ptr, int sign)
{

  struct data *cp = MALLOC_DATA;

  if (!cp)
    {
      SET_ERROR (MEM_ERROR);
    }


  if (ptr)
    {

      switch (ptr->ele_type)
	{

	case _INT:
	  cp->ele_type = _INT;

	  cp->INT_PTR = INSTALL_INT (ptr->INT_VALUE * sign);
	  break;

	case _STR:
	  cp->ele_type = _STR;
	  cp->STR_PTR = INSTALL_STRING (ptr->STR_VALUE);
	  break;

	case _FLT:
	  cp->ele_type = _FLT;
	  cp->FLT_PTR = INSTALL_FLOAT (ptr->FLT_VALUE * sign);
	  break;

	case _DBL:
	  cp->ele_type = _DBL;
	  cp->DBL_PTR = INSTALL_DOUBLE (ptr->DBL_VALUE * sign);
	  break;

	default:
	  free ((void *) cp);
	  SET_ERROR (UNKNOWN_DATATYPE);

	}

    }

  return cp;

}

/* The install routine that installs a particular data type*/

struct data *
install_data (struct data *ptr, char *value)
{

  /* Call the corresponding install of the data type and return a pointer to the data type */

  switch (ptr->ele_type)
    {

    case _INT:			//integer

      ptr->INT_PTR = INSTALL_INT (my_atoi (value));

      break;

    case _FLT:			//float

      ptr->FLT_PTR = INSTALL_FLOAT (atof (value));

      break;

    case _DBL:

      ptr->DBL_PTR = INSTALL_DOUBLE (atof (value));

      break;

    case _STR:

      ptr->STR_PTR = INSTALL_STRING (value);

      break;

    default:

      err_type = UNKNOWN_DATATYPE;

      error ();

    }				//switch


  return ptr;			//return a pointer to the data type after installing the data

}



/* The operation should be determined for objects through an abstract interface.*/

struct data *
operate_data (struct data *a, struct data *b, int op)
{

  struct data *ptr = (struct data *) NULL;

  struct data_operations *dptr;

  DataType dt = a->ele_type;	//get the element type

  CHECK_DPTR (dptr, dt);

  switch (op)
    {

    case PLUS:

      if (dptr->op_ptr->add_op)
	ptr = dptr->op_ptr->add_op (a, b);

      break;

    case MINUS:

      if (dptr->op_ptr->sub_op)
	ptr = dptr->op_ptr->sub_op (a, b);

      break;

    case MULTIPLY:

      if (dptr->op_ptr->mul_op)
	ptr = dptr->op_ptr->mul_op (a, b);

      break;

    case DIVIDE:

      if (dptr->op_ptr->div_op)
	ptr = dptr->op_ptr->div_op (a, b);

      break;

    case RS:

    case LS:

    case LT:

    case GT:

    case LTE:

    case GTE:

    case EE:

    case NE:

    case LOGICAL_AND:

    case LOGICAL_OR:

      if (dptr->op_ptr->compare_op)
	ptr = dptr->op_ptr->compare_op (a, b, op);

      break;

    case BITWISE_AND:

    case BITWISE_XOR:

    case BITWISE_OR:

      if (dptr->op_ptr->bit_op)
	ptr = dptr->op_ptr->bit_op (a, b, op);

      break;


    case LOGICAL_NOT:

      if (dptr->op_ptr->not_op)
	ptr = dptr->op_ptr->not_op (a);

      break;

    case ONES_COMPLEMENT:

      if (dptr->op_ptr->ones_complement)
	ptr = dptr->op_ptr->ones_complement (a);

      break;

    default:

      err_type = BAD_OPERATOR;

      error ();

    }

  if (!ptr)
    {

      err_type = OPERATION_NOT_SUPPORTED;

      error ();

    }

  return ptr;


}				//end routine       


/* Remove a data that is installed. Should be called after an expression parser finishes,and on a function epilogue,for clearing up the stack.*/

void
deinstall_data (struct data *ptr)
{

  if (ptr)
    {

      switch (ptr->ele_type)
	{

	case _INT:

	  deinstall_int (ptr);

	  break;

	case _FLT:

	  deinstall_float (ptr);

	  break;

	case _DBL:

	  deinstall_double (ptr);

	  break;

	case _STR:

	  deinstall_string (ptr);

	  break;

	default:

	  err_type = UNKNOWN_DATATYPE;

	  error ();

	}			//end switch


      free ((void *) ptr);	//free up the data 

    }

  return;

}


void
check_data (struct data_operations *dptr)
{

  if (!dptr->op_ptr)
    {

      err_type = UNKNOWN_DATATYPE;

      error ();

    }

}

/* Registering of data Type operation structure */

int
register_data (struct object_operations *op_ptr, int dtc)
{

  /* Register the operations for this data type */

  struct data_operations *dptr;

  if (dtc >= DATA_TYPES)

    {
      err_type = DATA_TYPE_OVERFLOW;
      error ();
    }


  dptr = &data_operations[dtc];

  dptr->op_ptr = op_ptr;	//set up the data operation pointer

  return 0;

}

/*Initialise the data types before registering */

void
initialise_datatypes ()
{
  struct data_operations *dptr = &data_operations[0];

  struct data_operations *traverse;

  for (traverse = dptr; traverse < dptr + DATA_TYPES; traverse++)

    traverse->op_ptr = (struct object_operations *) NULL;

}

/*Register all the available data types*/

void
register_all_datatypes (void)
{

  register_int ();		//register integer operations
  register_string ();		//register string operations
  register_float ();		//register float operations
  register_double ();		//register double operations

}
