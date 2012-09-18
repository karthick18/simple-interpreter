/* The integer object handling routine definitions */

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

/* Declaration of the integer operation structure */

struct data *int_not_op(struct data *a) {

  struct data *ptr = (struct data *) NULL;

  if(a->ele_type != _INT) {

    SET_ERROR(BAD_OPERAND);

  }

  ptr = MALLOC_DATA;
  if(! ptr) {
    SET_ERROR(MEM_ERROR);
  }

  ptr->ele_type = _INT;
  ptr->INT_PTR = INSTALL_INT(! a->INT_VALUE);
  
  return ptr;
 
}
    
struct data *int_ones_complement(struct data *a){

  struct data *ptr = (struct data *) NULL;

  if(a->ele_type != _INT) {

    SET_ERROR(BAD_OPERAND);

  }

  ptr = MALLOC_DATA;

  if(! ptr) {
 
    SET_ERROR(MEM_ERROR);

  }

  ptr->ele_type = _INT;

  ptr->INT_PTR = INSTALL_INT(~ a->INT_VALUE);

  return ptr;

}

struct data *int_add_op(struct data *a,struct data *b ){

  /* A wrapper for int_operate */

  return int_operate(a,b,PLUS) ;

}


struct data *int_sub_op(struct data *a,struct data *b ){

  return int_operate(a,b,MINUS);

}

struct data *int_mul_op(struct data *a,struct data *b){

  return int_operate(a,b,MULTIPLY);

}

struct data *int_div_op(struct data *a,struct data *b){

  return int_operate(a,b,DIVIDE);
}

struct data *int_compare_op(struct data *a,struct data *b,int op){

  return int_operate(a,b,op);

}

struct data *int_bit_op(struct data *a,struct data *b,int op) {

  return int_operate(a,b,op);

}


static struct object_operations operations = { 

  int_not_op,
  int_ones_complement,
  int_add_op,
  int_sub_op,
  int_mul_op,
  int_div_op,
  int_compare_op,
  int_bit_op,

/* Null terminated */

};


struct data* int_operate(struct data *a,struct data *b,int op){

  struct data *ptr = (struct data *) NULL;

  struct data_operations *dptr;

  switch(b->ele_type) {

    case _STR:

      CHECK_DPTR(dptr,_STR);

      switch(op) {

      case PLUS:
  
	if(dptr->op_ptr->add_op) 
	
	ptr = dptr->op_ptr->add_op(a,b);  

        else {

	  SET_ERROR(OPERATION_NOT_SUPPORTED);

        }


      break;

      case MULTIPLY:
     
        if(dptr->op_ptr->mul_op)        
        ptr = dptr->op_ptr->mul_op(a,b);

	else {

          SET_ERROR(OPERATION_NOT_SUPPORTED);

	}

        break;

      default:

        SET_ERROR(BAD_OPERATOR);

      }

      break;

    case _FLT:
      
      CHECK_DPTR(dptr,_FLT);

      switch(op) {

      case PLUS:
 
        if(dptr->op_ptr->add_op)
        
        ptr = dptr->op_ptr->add_op(a,b);
     
        else {

	  SET_ERROR(OPERATION_NOT_SUPPORTED);

	}

        break;

      case MINUS:
        
	if(dptr->op_ptr->sub_op)
        ptr = dptr->op_ptr->sub_op(a,b);

        else {

	  SET_ERROR(OPERATION_NOT_SUPPORTED);

	}

        break;

      case MULTIPLY:

        if(dptr->op_ptr->mul_op)
        ptr = dptr->op_ptr->mul_op(a,b);

        else {

	  SET_ERROR(OPERATION_NOT_SUPPORTED);

	}


        break;

      case DIVIDE:

        if(dptr->op_ptr->div_op) 
        ptr = dptr->op_ptr->div_op(a,b);
       
        else {

	  SET_ERROR(OPERATION_NOT_SUPPORTED);

        }

        break;

      case LT:
      case GT:
      case LTE:
      case GTE:
      case EE:
      case NE:
      case LOGICAL_AND:
      case LOGICAL_OR:

	if(dptr->op_ptr->compare_op)
          ptr = dptr->op_ptr->compare_op(a,b,op);

        else {

	  SET_ERROR(OPERATION_NOT_SUPPORTED);

	}

        break;

      default:

        SET_ERROR(BAD_OPERATOR);

      }

      break;
       
    case _DBL:

      CHECK_DPTR(dptr,_DBL);

      switch(op) {

      case PLUS:

	if(dptr->op_ptr->add_op)

	ptr = dptr->op_ptr->add_op(a,b);
 
        else {

	  SET_ERROR(OPERATION_NOT_SUPPORTED);

	}

      break;

      case MINUS:
     
        if(dptr->op_ptr->sub_op) 
	ptr = dptr->op_ptr->sub_op(a,b);

        else {

          SET_ERROR(OPERATION_NOT_SUPPORTED);

	}

        break;

      case MULTIPLY :

        if(dptr->op_ptr->mul_op)
        ptr = dptr->op_ptr->mul_op(a,b);
        else {

	  SET_ERROR(OPERATION_NOT_SUPPORTED);

	}
        break;

      case DIVIDE:

        if(dptr->op_ptr->div_op)
        ptr = dptr->op_ptr->div_op(a,b);
        else {

          SET_ERROR(OPERATION_NOT_SUPPORTED);

	}

        break;

      case LT:

      case GT:

      case LTE:
 
      case GTE:

      case EE:

      case NE:

      case LOGICAL_AND:

      case LOGICAL_OR:

	if(dptr->op_ptr->compare_op)
	  ptr = dptr->op_ptr->compare_op(a,b,op);

        else {

	  SET_ERROR(OPERATION_NOT_SUPPORTED);

	}

        break;


      default:

        SET_ERROR(BAD_OPERATOR);

      }

      break;

           
    case _INT:

      /* Make a data object by adding up the stuff */
      
      ptr = MALLOC_DATA;

      ptr->ele_type = _INT;

      switch(op) {

      case PLUS:

         ptr->INT_PTR = INSTALL_INT(a->INT_VALUE + b->INT_VALUE);
      
        break;

      case MINUS:

	ptr->INT_PTR = INSTALL_INT(a->INT_VALUE - b->INT_VALUE);

        break;

      case MULTIPLY:

        ptr->INT_PTR = INSTALL_INT(a->INT_VALUE * b->INT_VALUE);

        break;

      case DIVIDE:

        if (! (b->INT_VALUE) ) {
 
          ptr->INT_PTR = (struct int_type *) NULL;
          deinstall_data(ptr);
          
          SET_ERROR(DIV_BY_ZERO);

	}

        else 

	  ptr->INT_PTR = INSTALL_INT(a->INT_VALUE/b->INT_VALUE);
	
        break;
   
      case RS:
        ptr->INT_PTR = INSTALL_INT(a->INT_VALUE >> b->INT_VALUE);
        break;

      case LS:
        ptr->INT_PTR = INSTALL_INT(a->INT_VALUE << b->INT_VALUE);
        break;
     

      case GT:  //comparison operators

        ptr->INT_PTR = INSTALL_INT (a->INT_VALUE > b->INT_VALUE);

        break;


      case LT:

        ptr->INT_PTR = INSTALL_INT (a->INT_VALUE < b->INT_VALUE);

        break;

      case LTE:

        ptr->INT_PTR = INSTALL_INT(a->INT_VALUE <= b->INT_VALUE);

        break;

      case GTE:

        ptr->INT_PTR = INSTALL_INT(a->INT_VALUE >= b->INT_VALUE);

        break;

      case EE:
       
        ptr->INT_PTR = INSTALL_INT(a->INT_VALUE == b->INT_VALUE);
     
        break;

      case NE:

        ptr->INT_PTR = INSTALL_INT(a->INT_VALUE != b->INT_VALUE);

        break;


      case BITWISE_AND :

        ptr->INT_PTR = INSTALL_INT (a->INT_VALUE & b->INT_VALUE);

        break;

      
      case BITWISE_XOR :

        ptr->INT_PTR = INSTALL_INT (a->INT_VALUE ^ b->INT_VALUE);

        break;

      case BITWISE_OR :

        ptr->INT_PTR = INSTALL_INT (a->INT_VALUE | b->INT_VALUE);

        break;

      case LOGICAL_AND:

        ptr->INT_PTR = INSTALL_INT (a->INT_VALUE && b->INT_VALUE);

        break;

      case LOGICAL_OR:

        ptr->INT_PTR = INSTALL_INT(a->INT_VALUE || b->INT_VALUE);

        break;

      default:

	SET_ERROR(BAD_OPERATOR);

      }
 
      break;

    default :

      SET_ERROR(UNKNOWN_DATATYPE);

    } //end switch

  return ptr;

} //end routine  


struct int_type *install_int(int value){

  struct int_type *i;

  i = MALLOC_INT; //allocate an integer object
 
  if( !i) 
 
    {

      SET_ERROR(MEM_ERROR);

    }

  i->value = value;

  i->int_ptr = &operations ; //set up the operation pointer

  return i; 

}     


struct data *deinstall_int(struct data *ptr) {

  if( ptr) {

    if(ptr->INT_PTR)

      free((void*)ptr->INT_PTR);  

  }

  return ptr;

}




int register_int(void) {

  /* Register the data type */

  return register_data(&operations,_INT);

}


         
  



