#ifndef _INTERN_H
#define _INTERN_H 1

#define BUFFER 1024

#define INTERN 10

#include<data.h>
#include<arg.h>
#include<string.h>

extern int do_print (char *buf, const char *fmt, struct argument *arg);

extern struct data *my_generic_printf (struct argument *arg, int flag);

struct intern
{
  char *keyword;
  struct data *(*intern_ptr) (unsigned long, int, int);
};

extern struct intern *lookup_intern (char *keyword);

extern struct argument *builtin_handler (unsigned long lookup_stop,
					 int scope);

extern struct data *printf_handler (unsigned long lookup_stop, int scope,
				    int status);

extern struct data *sprintf_handler (unsigned long lookup_stop, int scope,
				     int status);

extern struct data *output_handler (unsigned long lookup_stop, int scope,
				    int status, int flag);

int my_strnlen (const char *s, int precision);

#endif
