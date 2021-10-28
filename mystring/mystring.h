#ifndef _MYSTRING_H
#define _MYSTRING_H 1

#include<stdio.h>

#include<data.h>

#define STRING_FIND 	1
#define STRING_REPLACE  2
#define STRING_STRCHR   3
#define STRING_STRRCHR  4
#define STRING_STRCMP   5
#define STRING_STRNCMP  6
#define STRING_STRCASECMP 7
#define STRING_STRCPY  8
#define STRING_STRNCPY 9

#define INSTALL_STRING(value) install_string(value)

#define FREE_STRING(data) deinstall_string(data)	//free up an integer data

/* Declarations of installation and free routines,plus the declaration of the most important structure for definition of data types */


struct str_type
{

  char *value;

  struct object_operations *str_ptr;

};

extern void str_replace (char *src, char *what, char *by);
extern struct str_type *install_string (char *value);
extern struct data *deinstall_string (struct data *ptr);
extern struct data *str_operate (struct data *a, struct data *b, int op);
extern int str_convert (struct data *d, char *s);
extern int register_string (void);

#endif
