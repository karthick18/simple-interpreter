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

#define BUFFER 1024

extern Error err_type;


struct data *str_add_op(struct data *a,struct data *b ){

  /* A wrapper for int_operate */

  return str_operate(a,b,PLUS) ;

}



struct data *str_mul_op(struct data *a,struct data *b){

  return str_operate(a,b,MULTIPLY);

}

int str_convert(struct data *ptr,char *s) {
 
  int ret=0;

  switch(ptr->ele_type ) {

  case _INT:
   
    sprintf(s,"%d",ptr->INT_VALUE);
    break;

  case _FLT:
    sprintf(s,"%f",ptr->FLT_VALUE);
    break;

  case _DBL:
    sprintf(s,"%f",ptr->DBL_VALUE);
    break;

  case _STR:
    ret = 1;
    break;
  default:
    SET_ERROR(UNKNOWN_DATATYPE);

  }

  return ret;

}//end routine

struct data *str_find_op(struct data *src,struct data *in,int nbytes){

  struct data *ptr = (struct data *) NULL;

  char src_buf[BUFFER]; 

  char dst_buf[BUFFER];

  char *sptr = src_buf;
  char *dptr = dst_buf;
  char *s ;
   
  if (str_convert(src,sptr) )
    sptr = src->STR_VALUE;
  
  if(str_convert(in,dptr) ) 
    dptr = in->STR_VALUE;

  s = sptr + nbytes;

  if (s < sptr + strlen(sptr) ) {
 
    s = strstr(s,dptr); //try finding dptr in s

    if (s) {

      ptr = MALLOC_DATA;

      ptr->ele_type = _STR;

      ptr->STR_PTR = INSTALL_STRING(s); 
        
    }
      
  }
  
  return ptr;

}

struct data *str_compare_op(struct data *src,struct data *in,int nbytes){

  struct data *ptr = (struct data *) NULL;

  char src_buf[BUFFER]; 

  char dst_buf[BUFFER];

  char *sptr = src_buf;
  char *dptr = dst_buf;
  char *s ;
  char *d;
  int res;

  if   (str_convert(src,sptr) )
    sptr = src->STR_VALUE;
  if (str_convert(in,dptr) )
    dptr = in->STR_VALUE;

  s = sptr + nbytes;
  d = dptr + nbytes;

  if (s >= sptr + strlen(sptr) )
    s  = sptr;

  if (d >= dptr + strlen(dptr) )
    d = dptr;

  res = strcmp(s,d); //compare the strings 

  //Now create an integer data object from the result

  ptr = MALLOC_DATA;

  ptr->ele_type = _INT;

  ptr->INT_PTR = INSTALL_INT(res); 

  return ptr;   

}


struct data *str_replace_op(struct data *src,struct data *what,struct data *by){
 
  struct data *ptr = (struct data *) NULL;

  char src_buf[BUFFER];
  char what_buf[BUFFER];
  char by_buf[BUFFER];
  char *src_ptr = src_buf;
  char *by_ptr = by_buf;
  char *what_ptr = what_buf;
  int src_flag = 0;

  if (str_convert(src,src_ptr) ) {

    src_ptr = strdup(src->STR_VALUE);

    src_flag = 1;

  }

  if (str_convert(what,what_ptr) )
   
    what_ptr = what->STR_VALUE;

  if (str_convert(by,by_ptr) )

    by_ptr = by->STR_VALUE ;

  str_replace(src_ptr,what_ptr,by_ptr); 

  ptr = MALLOC_DATA;
  ptr->ele_type = _STR;
  ptr->STR_PTR = INSTALL_STRING(src_ptr);
 
  if (src_flag) 

    free((void*) src_ptr);

  return ptr;

}
  

void str_replace(char *src,char *what,char *by) {

  char *ptr;

  while( (ptr = strstr(src,what) ) ){

    memmove(ptr+strlen(by),ptr+strlen(what), strlen(ptr+strlen(what) ) + 1);
    memcpy(ptr,by,strlen(by));

  }

}


static struct object_operations operations = { 

  NULL,
  NULL,
  str_add_op,
  NULL,
  str_mul_op,
  NULL,
  str_compare_op,
  NULL,
  str_find_op,
  str_replace_op,
  
};


struct str_type *install_string(char *value) {

  struct str_type *s;

  if(!value) {

    SET_ERROR(BAD_OPERAND);

  }

  s = MALLOC_STR;

  if(! s) {

    SET_ERROR(MEM_ERROR);

  }

  s->value = strdup(value);
 
  s->str_ptr = &operations;

  return s;

}


struct data* str_operate(struct data *a,struct data *b,int op){

  struct data *ptr = (struct data *) NULL;
  char src_buf[BUFFER];
  char dst_buf[BUFFER];
  char *src_ptr =src_buf;
  char *dst_ptr =dst_buf;
  char *res;
  char *temp;
  int i,n=0;

  if( str_convert(a,src_ptr) )
    src_ptr = a->STR_VALUE;
  if(str_convert(b,dst_ptr) )
    dst_ptr  = b->STR_VALUE;

   ptr = MALLOC_DATA;

    if(! ptr) {

      SET_ERROR(MEM_ERROR);

    }

    ptr->ele_type = _STR;

    switch(op) { 

   case PLUS: //add 2 strings

    res = (char *)malloc(sizeof(char) * (strlen(src_ptr) + strlen(dst_ptr) + 1) );
    if (!res) {

      SET_ERROR(MEM_ERROR);

    }

    strcpy(res,src_ptr);
    strcat(res,dst_ptr);
    
    ptr->STR_PTR = INSTALL_STRING(res); 

    free((void*)res); 
    
    break;

  case MULTIPLY: //multiply 2 strings

    if((a->ele_type == _INT) || (b->ele_type == _INT)  ){

      if(a->ele_type == _INT) {
	n=atoi(src_ptr);
        temp=dst_ptr;
      }
      else {

        n=atoi(dst_ptr);
        temp=src_ptr;
      }

      res = (char*)malloc(sizeof(char) * (strlen(temp) * n + 1) );
     
      if(! res) {

	SET_ERROR(MEM_ERROR);

      }
 
      *res = 0;

      for(i=0;i < n; i++) 
     
	strcat(res,temp);
      
      ptr->STR_PTR = INSTALL_STRING(res);
      free((void*)res);
           
    } 

    else {

      SET_ERROR(BAD_OPERAND);

    }

    break;

  default:
   
    ptr->STR_PTR = (struct str_type *) NULL;

    deinstall_data(ptr);

    SET_ERROR(BAD_OPERATOR);
   

  } //end switch
           
  return ptr;

  } //end routine  


struct data *deinstall_string(struct data *ptr) {

  
  if( ptr) {

    if(ptr->ele_type == _STR) {

    if(ptr->STR_PTR)
    
      if (ptr->STR_VALUE) free((void*)ptr->STR_VALUE);

      free((void*)ptr->STR_PTR);  

    }

  }

  return ptr;

}




int register_string(void) {

  /* Register the data type */

  return register_data(&operations,_STR);

}


         
  



