#ifndef _MYFLOAT_H 
#define _MYFLOAT_H 1

#include<stdio.h>

#include<data.h>

#define INSTALL_FLOAT(value) install_float(value) 

#define FREE_FLOAT(data) deinstall_float(data) 

/* Declarations of installation and free routines,plus the declaration of the most important structure for definition of data types */


struct flt_type {

  float value; 

  struct object_operations *flt_ptr;

};


extern struct flt_type *install_float(float value);

extern struct data *deinstall_float(struct data *data);

extern int register_float(void);

extern struct data *flt_operate(struct data *a,struct data *b,int op);

#endif
  




