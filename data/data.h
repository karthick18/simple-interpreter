/* Seamless integration of Data */

#ifndef _DATA_H 
#define _DATA_H 1

#define DATA_TYPES 0xFF

#include<stdio.h>

#include<myint.h>

#include<myfloat.h>

#include<mydouble.h>

#include<mystring.h>

#include <utils.h>

typedef enum {

  _INT,
  _FLT,
  _DBL,
  _STR

}DataType;



/* A Macro that installs the data type when given a value */

#define INSTALL_DATA(ptr,value) install_data(ptr,value)

#define OPERATE_DATA(a,b,what)  operate_data(a,b,what)

#define DEINSTALL_DATA(data)    deinstall_data(data)

/* Shortcuts to pointer accesses */

#define INT_PTR elements.int_type

#define FLT_PTR elements.flt_type

#define DBL_PTR elements.dbl_type

#define STR_PTR elements.str_type

/* Shortcuts to memory allocation for these data structures*/

#define MALLOC_DATA  (struct data *) malloc(sizeof(struct data) )

#define MALLOC_INT   (struct int_type *) malloc(sizeof(struct int_type))

#define MALLOC_FLT   (struct flt_type *) malloc(sizeof(struct flt_type) )

#define MALLOC_DBL   (struct dbl_type *) malloc(sizeof(struct dbl_type) )

#define MALLOC_STR   (struct str_type *) malloc(sizeof(struct str_type) )

/*Shortcuts to data types pointers*/

#define INT_DATA_PTR(ptr)  (ptr)->INT_PTR

#define FLT_DATA_PTR(ptr)  (ptr)->FLT_PTR

#define DBL_DATA_PTR(ptr)  (ptr)->DBL_PTR

#define STR_DATA_PTR(ptr)  (ptr)->STR_PTR


/* Shortcuts to value accesses:Should be called using a data pointer preferably */

#define INT_VALUE    INT_PTR->value

#define FLT_VALUE    FLT_PTR->value

#define DBL_VALUE    DBL_PTR->value

#define STR_VALUE    STR_PTR->value

/* Shortcuts to operation structure pointer for the data types*/

#define INT_OPERATION_STR INT_PTR->int_ptr

#define FLT_OPERATION_STR FLT_PTR->flt_ptr

#define DBL_OPERATION_STR DBL_PTR->dbl_ptr

#define STR_OPERATION_STR STR_PTR->str_ptr

/*Shortcuts to the operation pointers for the data types*/

#define INT_OPERATION INT_OPERATION_STR->into_ptr

#define FLT_OPERATION FLT_OPERATION_STR->flto_ptr

#define DBL_OPERATION DBL_OPERATION_STR->dblo_ptr

/* A bit different for strings because of heterogenous return types.*/

#define STR_OPERATION_ADD    STR_OPERATION_STR->str_add

#define STR_OPERATION_MUL    STR_OPERATION_STR->str_mul

#define STR_OPERATION_FIND   STR_OPERATION_STR->str_find

#define STR_OPERATION_REPLACE STR_OPERATION_STR->str_replace

#define STR_OPERATION_COMPARE STR_OPERATION_STR->str_compare     


/* Typecasting for get_data routine */

#define INT_ADD_TYPECAST  (struct data * (*) (struct data *,struct data *) )

#define INT_SUB_TYPECAST  INT_ADD_TYPECAST

#define INT_MUL_TYPECAST  INT_ADD_TYPECAST

#define INT_DIV_TYPECAST  INT_ADD_TYPECAST

#define INT_COMPARE_TYPECAST (struct data * (*)(struct data *,struct data *,int) )

#define INT_BIT_TYPECAST     INT_COMPARE_TYPECAST

#define FLT_ADD_TYPECAST     INT_ADD_TYPECAST

#define FLT_SUB_TYPECAST     INT_ADD_TYPECAST

#define FLT_MUL_TYPECAST     INT_ADD_TYPECAST

#define FLT_DIV_TYPECAST     INT_ADD_TYPECAST

#define DBL_ADD_TYPECAST     INT_ADD_TYPECAST

#define DBL_SUB_TYPECAST     INT_ADD_TYPECAST

#define DBL_MUL_TYPECAST     INT_ADD_TYPECAST

#define DBL_DIV_TYPECAST     INT_ADD_TYPECAST

#define STR_ADD_TYPECAST     INT_ADD_TYPECAST

#define STR_MUL_TYPECAST     INT_ADD_TYPECAST

#define STR_FIND_TYPECAST    INT_COMPARE_TYPECAST

#define STR_COMPARE_TYPECAST INT_COMPARE_TYPECAST

#define STR_REPLACE_TYPECAST (struct data * (*) (struct data *,struct data *,struct data *) )


#define CHECK_DPTR(dptr,dt)     dptr=&data_operations[dt]; check_data(dptr)

#define SET_ERROR(type)  err_type = type; error()


/*Inline function definition:To fetch the appropriate Data Type given a Pointer shortcut:Used by Print routines */

#define EVAL_DATA(data,ps) ( \
{ \
data->ps; \
}\
)

/* Define the abstract data type structure */

/* A Seamless integration of data types*/
 
struct data {

  DataType ele_type;

  union elements {

    struct int_type *int_type;
    struct flt_type *flt_type;
    struct dbl_type *dbl_type;
    struct str_type *str_type;

  }elements;

};

/* Operation gateway for Data Types: A wrapper for each data type: */

/* Each data type need to register their object operations for accesses */


struct data_operations {

  struct object_operations *op_ptr;

};

extern struct data_operations data_operations[DATA_TYPES];

struct object_operations {

  struct data * (*not_op) (struct data *a); 
  
  struct data * (*ones_complement)(struct data *a);

  struct data * (*add_op) (struct data *a,struct data *b); //add operation
 
  struct data * (*sub_op) (struct data *a,struct data *b); //sub operation on a data type

  struct data * (*mul_op) (struct data *a,struct data *b); //mul operation

  struct data * (*div_op) (struct data *a,struct data *b); //div operation

  struct data * (*compare_op)(struct data *a,struct data *b,int op); //compare operation

  struct data * (*bit_op) (struct data *a,struct data *b,int op); //bit operations

  /* The below operations will be string objects */

  struct data * (*find_op)(struct data *in,struct data *find,int index);

  struct data * (*replace_op) (struct data *in,struct data *what,struct data *by);

}; //end of pointer table


/* Gateway Routines*/

extern struct data *copy_data(struct data *ptr,int sign);
extern struct data *install_data(struct data *ptr,char *value);
extern struct data *operate_data(struct data *a,struct data *b,int what);
extern void deinstall_data(struct data *ptr);
extern int register_data(struct object_operations *ops,int dt);
extern void check_data(struct data_operations *dptr);
extern void initialise_datatypes(void);
extern void register_all_datatypes(void);
extern void show_data(struct data *ptr);
#endif





