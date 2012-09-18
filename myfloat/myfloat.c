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


struct data *flt_not_op(struct data *a) {

  struct data *ptr = (struct data *) NULL;

  if(a->ele_type != _FLT) {

    SET_ERROR(BAD_OPERAND);

  }

  ptr = MALLOC_DATA;
  if(! ptr) {
    SET_ERROR(MEM_ERROR);
  }

  ptr->ele_type = _FLT;
  ptr->FLT_PTR = INSTALL_FLOAT(! a->FLT_VALUE);
  
  return ptr;
 
}


struct data *flt_add_op(struct data *a,struct data *b ){

  /* A wrapper for int_operate */

  return flt_operate(a,b,PLUS) ;

}


struct data *flt_sub_op(struct data *a,struct data *b ){

  return flt_operate(a,b,MINUS);

}

struct data *flt_mul_op(struct data *a,struct data *b){

  return flt_operate(a,b,MULTIPLY);

}

struct data *flt_div_op(struct data *a,struct data *b){

  return flt_operate(a,b,DIVIDE);
}

struct data *flt_compare_op(struct data *a,struct data *b,int op){
  return flt_operate(a,b,op);
}

static struct object_operations operations = { 

  flt_not_op,
  NULL,
  flt_add_op,
  flt_sub_op,
  flt_mul_op,
  flt_div_op,
  flt_compare_op,
/* Null terminated */

};


struct data* flt_operate(struct data *a,struct data *b,int op){

  struct data *ptr = (struct data *) NULL;

  struct data_operations *dptr;

  float x,y=0;

  if (a->ele_type == _INT) 

    x = (float) a->INT_VALUE;
 
  else 
 
    x = a->FLT_VALUE;

 
  if(b->ele_type == _INT) 

      y = (float)b->INT_VALUE;

  else if(b->ele_type == _FLT)
 
      y = b->FLT_VALUE;
    
  switch(b->ele_type) {

    case _STR:

      CHECK_DPTR(dptr,_STR);

      switch(op) {

      case PLUS:
	if (dptr->op_ptr->add_op)
        	ptr = dptr->op_ptr->add_op(a,b);  

        else  {

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
      
    case _INT:   

      ptr = MALLOC_DATA;

      if(!ptr) {

        SET_ERROR(MEM_ERROR);

      }
      ptr->ele_type = _FLT;
  
      switch(op) {

      case PLUS:
        
        /* add the two data and get a new data */
         
         ptr->FLT_PTR = INSTALL_FLOAT(x+y);
                     
        break;

      case MINUS:

          ptr->FLT_PTR = INSTALL_FLOAT(x-y);

        break;

      case MULTIPLY:

         ptr->FLT_PTR = INSTALL_FLOAT(x*y);

     
        break;

      case DIVIDE:

        if(! y)
       {

           SET_ERROR(DIV_BY_ZERO);

        }

        ptr->FLT_PTR = INSTALL_FLOAT(x/y);
       
       break;         

      case LT:
 
        ptr->FLT_PTR = INSTALL_FLOAT(x<y);
        break;
       
      case GT:
        ptr->FLT_PTR = INSTALL_FLOAT(x>y);
        break;

      
      case LTE:

        ptr->FLT_PTR = INSTALL_FLOAT(x<=y);

        break;

      case GTE:

        ptr->FLT_PTR = INSTALL_FLOAT(x>=y);
        break;

      case EE:
        ptr->FLT_PTR = INSTALL_FLOAT(x==y);

        break;

      case NE:

        ptr->FLT_PTR = INSTALL_FLOAT(x!=y);
        break;  

      case LOGICAL_AND:
        ptr->FLT_PTR = INSTALL_FLOAT(x && y);
        break;

      case LOGICAL_OR:
        ptr->FLT_PTR = INSTALL_FLOAT(x || y);
        break;

     default:
      
      ptr->FLT_PTR = (struct flt_type*) NULL;
      deinstall_data(ptr);

      SET_ERROR(BAD_OPERATOR);  
      
      }

      break;
       
    case _DBL:

      CHECK_DPTR(dptr,_DBL);

      switch(op) {

      case PLUS:

        if (dptr->op_ptr->add_op) 
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
          ptr=dptr->op_ptr->compare_op(a,b,op);
        else {
	  SET_ERROR(OPERATION_NOT_SUPPORTED);
	}
	break;      
    
      default:

        err_type = BAD_OPERATOR;

        error();

      }

 
   break;

 default:

     SET_ERROR(UNKNOWN_DATATYPE);

 } //end outer switch

return ptr;
 
} //end routine  


struct flt_type *install_float(float value){

  struct flt_type *f;

  f = MALLOC_FLT; //allocate an integer object
 
  if( !f) 
 
    {

      SET_ERROR(MEM_ERROR);      

    }

  f->value = value;

  f->flt_ptr = &operations ; //set up the operation pointer

  return f; 

}     


struct data *deinstall_float(struct data *ptr) {

  
  if( ptr) {

    if(ptr->ele_type == _FLT) {

    if(ptr->FLT_PTR)

      free((void*)ptr->FLT_PTR);  

    }

  }

  return ptr;

}




int register_float(void) {

  /* Register the data type */

  return register_data(&operations,_FLT);

}


         
  



