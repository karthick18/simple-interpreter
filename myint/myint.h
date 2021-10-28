#ifndef _MYINT_H
#define _MYINT_H 1

#include<stdio.h>

#include<data.h>


#define INSTALL_INT(value) install_int(value)	//install an integer data type

#define FREE_INT(data) deinstall_int(data)	//free up an integer data

/* Declarations of installation and free routines,plus the declaration of the most important structure for definition of data types */


struct int_type
{

  int value;

  struct object_operations *int_ptr;

};


extern struct int_type *install_int (int value);

extern struct data *deinstall_int (struct data *);

/* The routine for integer operation */

extern struct data *int_operate (struct data *a, struct data *b, int what);

extern int register_int (void);
#endif
