/*
 *  linux/lib/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)

A.R.Karthick tuned it for his own Benefits: Cheap Stuff.

 */

#include<stdio.h>
#include<intern.h>
#include<data.h>
#include<string.h>
#include<stdlib.h>

/* we use this so that we can do without the ctype library */

#define is_digit(c)	((c) >= '0' && (c) <= '9')

static int lasu_atoi(const char **s)
{
	int i=0;

	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */

#define do_div(n,base) ({ \
int __res; \
__res = ((unsigned long) n) % (unsigned) base; \
n = ((unsigned long) n) / (unsigned) base; \
__res; })

static char * number(char * str, long num, int base, int size, int precision
	,int type)
{
	char c,sign,tmp[66];
	const char *digits="0123456789abcdefghijklmnopqrstuvwxyz";
	int i;

	if (type & LARGE)
		digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN) {
		if (num < 0) {
			sign = '-';
			num = -num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num != 0)
		tmp[i++] = digits[do_div(num,base)];
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT)))
		while(size-->0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	if (type & SPECIAL) {
		if (base==8)
			*str++ = '0';
		else if (base==16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	}
	if (!(type & LEFT))
		while (size-- > 0)
			*str++ = c;
	while (i < precision--)
		*str++ = '0';
	while (i-- > 0)
		*str++ = tmp[i];
	while (size-- > 0)
		*str++ = ' ';
	return str;
}


int do_print(char *buf, const char *fmt, struct argument *ptr)
{
	int len;
	unsigned long num;
	int i, base;
	char * str;
	const char *s;
        char buffer[BUFFER+1];

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */

	for (str=buf ; *fmt ; ++fmt) {
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
			
		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
				}
		
		/* get field width */
		field_width = -1;
		if (is_digit(*fmt))
			field_width = lasu_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = EVAL_DATA(arg_next(ptr,_INT),INT_VALUE);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;	
			if (is_digit(*fmt))
				precision = lasu_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = EVAL_DATA(arg_next(ptr,_INT),INT_VALUE);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		/* default base */
		base = 10;

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) EVAL_DATA(arg_next(ptr,_INT),INT_VALUE);
			while (--field_width > 0)
				*str++ = ' ';
			continue;

		case 's':
			s = EVAL_DATA(arg_next(ptr,_STR), STR_VALUE);
			if (!s)
				s = "<NULL>";

			len = my_strnlen(s, precision);

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			continue;

                case 'f':
     
		  len = sprintf(buffer,"%*.*f",field_width,precision,EVAL_DATA(arg_next(ptr,_FLT),FLT_VALUE) );
				if(! (flags & LEFT) ) 
			         while( len < field_width --)
          				*str++= ' ';
          
                                for(i=0;i < len; i ++)
				 *str++ = buffer[i];
                                 
                                while(len < field_width --)
		        		*str++ =  ' ';
         
                                continue;
 
                                                                  
		case 'p':
			if (field_width == -1) {
				field_width = 2*sizeof(void *);
				flags |= ZEROPAD;
			}
			str = number(str,
				(unsigned long) arg_next(ptr, -1), 16,
				field_width, precision, flags);
			continue;


		case '%':
			*str++ = '%';
			continue;

		/* integer number formats - set up the flags and "break" */
		case 'o':
			base = 8;
			break;

		case 'X':
			flags |= LARGE;
		case 'x':
			base = 16;
			break;
                
		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			break;

		default:
			*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			continue;
		}
		if (qualifier == 'l')
			num = EVAL_DATA(arg_next(ptr,_INT),INT_VALUE);
		else if (qualifier == 'h') {
		  num = (unsigned short) EVAL_DATA(arg_next(ptr,_INT),INT_VALUE);
			if (flags & SIGN)
				num = (short) num;
		} else 
		  num = EVAL_DATA(arg_next(ptr,_INT),INT_VALUE);
		
			
		str = number(str, num, base, field_width, precision, flags);
	}
	*str = '\0';
	return str-buf;
}

struct data *my_generic_printf(struct argument *ptr,int flag)
{
  struct data *dp ;
  int i;
  char buffer[BUFFER + 1];

  if(ptr->head->var->value->ele_type != _STR){ 

    arg_end(ptr);

    SET_ERROR(PRINT_SYNTAX_ERROR);
  }

  arg_start(ptr); //set up the argument start
  
  i = do_print(buffer,ptr->tail->var->value->STR_VALUE,ptr);

  arg_end(ptr);

  if(! flag ) { 

    printf("%s",buffer); //display the buffer for PRINTF

    sprintf(buffer,"%d",i);
  }

  dp  = MALLOC_DATA;

  if(! dp) {

    SET_ERROR(MEM_ERROR);

  }
 
  dp->ele_type = flag ? _STR : _INT;

  return install_data(dp,buffer);
  
      
}

int my_strnlen(const char *s,int max) {
  const char *ptr = s;
  for(; max-- && *ptr ; ptr++ );
  return ptr - s;

}







