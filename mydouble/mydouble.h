#ifndef _MYDOUBLE_H
#define _MYDOUBLE_H 1

#include<stdio.h>

#include<data.h>


#define INSTALL_DOUBLE(value) install_double(value)

#define FREE_DOUBLE(data) deinstall_double(data)

/* Declarations of installation and free routines,plus the declaration of the most important structure for definition of data types */


struct dbl_type
{

  double value;

  struct object_operations *dbl_ptr;

};


extern struct dbl_type *install_double (double value);

extern struct data *deinstall_double (struct data *data);

extern struct data *dbl_operate (struct data *a, struct data *b, int what);

extern int register_double (void);

#endif
