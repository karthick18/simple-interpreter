#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>

#include<utils.h>
#include<token.h>
#include<error.h>
#include<fun.h>
#include <data.h>
#include<stack.h>
#include <lval.h>
#include<scope.h>
#include<exp.h>
#include<fun.h>
#include<keywords.h>
#include<ctype.h>
#include<intern.h>

struct keywords keywords[KEYWORDS] = {

  {"return", return_handler},
  {"fun", fun_handler},
  {"if", if_handler},
  {"while", while_handler},
  {"break", break_handler},
  {"continue", continue_handler},
  {"do", dowhile_handler},
  {"for", for_handler},
  {NULL, NULL},
};

struct intern intern[INTERN] = {

  {"printf", printf_handler},
  {"sprintf", sprintf_handler},

  /* Aliases for printf and sprintf */

  {"print", printf_handler},

  {"sprint", sprintf_handler},

  {NULL, NULL},

};

struct keywords *
lookup_keyword (char *keyword)
{

  struct keywords *ptr;

  for (ptr = &keywords[0]; ptr->keyword; ptr++)
    if (!strcmp (ptr->keyword, keyword))
      return ptr;

  return (struct keywords *) NULL;

}

struct intern *
lookup_intern (char *keyword)
{

  struct intern *ptr;

  for (ptr = &intern[0]; ptr->keyword; ptr++)
    if (!strcmp (ptr->keyword, keyword))
      return ptr;

  return (struct intern *) NULL;

}

char *
initialise (int argc, char *argv[])
{
  char *filename = NULL;

  if (argc != 1 && argc != 2)
    {
      err (0, "Insufficient Arguments:");
      return NULL;
    }
  if (argc == 2)
    {
      filename = argv[1];
    }
  return read_file (filename);	//read the data in the filename and return the data
}

void
err (int down, char *fmt, ...)
{

  char buffer[BUFFER];
  va_list ptr;

  va_start (ptr, fmt);

  vsnprintf (buffer, BUFFER - 1, fmt, ptr);

  va_end (ptr);

  fprintf (stderr, "%s\n", buffer);

  if (down)

    exit (0);

}


void
my_strncpy (char *dst, char *src, unsigned int bytes)
{

  while (bytes-- > 0 && *src)

    *dst++ = *src++;

  *dst = 0;

}

/* Return the base of the string */

int
determine_base (char **s)
{

  int base = 10;

  if (**s == '0' && ((*s)[1]) != '.')
    {
      (*s)++;
      base = 8;
      if (tolower (**s) == 'x' && isxdigit ((*s)[1]))
	{
	  (*s)++;
	  base = 16;
	}
      else if (tolower (**s) == 'b' && isdigit ((*s)[1]))
	{
	  (*s)++;
	  base = 2;
	}

    }

  return base;
}


/* Converts string into integer based on base 16,10,8 and 2 */

int
my_atoi (char *s)
{
  int sign = 0, value, result, base;

  while (isspace (*s))
    s++;			//skip spaces

  sign = (*s == '-') ? -1 : (*s == '+') ? 1 : sign;

  if (sign)
    s++;

  else
    sign = 1;

  while (isspace (*s))
    s++;

  base = determine_base (&s);

  for (result = 0; isxdigit (*s); s++)
    {

      value =
	isdigit (*s) ? *s - '0' : (islower (*s) ? toupper (*s) : *s) - 'A' +
	10;
      if (value >= base)
	{

	  SET_ERROR (BAD_NUMERIC_CONSTANT);
	}

      result = result * base + value;	//set up the result

    }

  return result * sign;

}


char *
read_file (char *filename)
{

  int fd = 0;			/*std input by default */

  char *start = NULL;

  unsigned int blocks = 1, nbytes = 0, total_read = 0;

  unsigned int block_size = BUFFER;

  if (filename)
    {
      fd = open (filename, O_RDONLY);

      if (fd < 0)
	{
	  return NULL;
	}
    }

  start = malloc (blocks * block_size + 1);

  if (!start)
    goto out_close;

  err (0, "Reading File %s", filename ? filename : "null");

  while ((nbytes = read (fd, start + total_read, block_size)) > 0)
    {
      total_read += nbytes;
      ++blocks;
      start = realloc (start, blocks * block_size + 1);
      if (!start)
	goto out_close;
    }

  start[total_read] = 0;
  if (blocks * block_size + 1 > total_read)
    {
      memset (start + total_read, 0, (blocks * block_size + 1) - total_read);
    }
out_close:
  if (fd > 0)
    close (fd);

  return start;
}

int
examine_token (void)
{

  return token_info ();

}

/* This one hunts for functions and marks their start and end points */

int
function_precursor (void)
{

  /*Loop through the Program and check for Function definitions */

  int num_fun = 0, nb = 0;

  char fun_name[FUN_NAME + 1];

  unsigned long start, end, num;

  Token t;

  extern char tok[];		//the token value

  extern char *reference;	//the program reference pointer

  int gb = 0;

  while (1)
    {

      switch (get_token ())
	{

	case IDENTIFIER:	//check for a function occurence

#ifdef DEBUG

	  token_info ();

#endif

	  if (ISFUN (tok))
	    {

	      /*If its a function token check for the last token */

	      if (!VALID_FUN ())
		{

		  err_type = SYNTAX_ERROR;

		}

	      /*Now Check for Functional Syntaxes */

	      else if (*reference != ' ')

		err_type = SYNTAX_ERROR;

	      else
		{

		  //get a token. 

		  switch (get_token ())
		    {

		    default:


		      err (0, "Function Precursor:Syntax Error at line %d",
			   lines + 1);

		      SET_ERROR (SYNTAX_ERROR);


		    case IDENTIFIER:

		      my_strncpy (fun_name, tok, FUN_NAME);

		      switch (get_token ())
			{	//get a token again


			default:


			  err (0,
			       "Function Precursor:Syntax Error at line %d\n",
			       lines + 1);

			  SET_ERROR (SYNTAX_ERROR);


			case LP:	//hit a Left Parenthesis

			  //set up the Function start

			  start = (unsigned long) reference;

			  num = argument_parser ();	//checks the arguments

			  block_start ();

			  if (current_token != LB)

			    err_type = SYNTAX_ERROR;

			  else
			    {

			      //count the number of braces

			      nb++;	//increment the braces count

			      while (nb)
				{	//parsing a block

				  t = get_token ();

				  if (t == LB)
				    ++nb;

				  else if (t == RB)
				    --nb;	//decrement the number braces


				  else if (t == END)
				    {

				      err_type = SYNTAX_ERROR;

				      break;

				    }

				}	//end while

			    }	//end else 


			}	//end inner switch         

		    }		//end outer switch

		}		//end inner else

	      if (err_type != OK)
		{

#ifdef DEBUG

		  err (0, "Function Precursor:Syntax Error at line %d",
		       lines + 1);

#endif

		  SET_ERROR (err_type);

		}

	      end = (unsigned long) reference;	//mark the end

	      //now store the required into the function hash_table

	      if (!install_fun (fun_name, start, end, num))
		{

#ifdef DEBUG

		  err (0,
		       "Function Precuror: Error in installing Function:\n");

#endif

		  SET_ERROR (MEM_ERROR);

		}

	      else
		{

#ifdef DEBUG
		  err (0,
		       "Finished Installing Function %s at Start Address:%#x and End Address:%#x",
		       fun_name, start, end);

#endif

		}

	      num_fun++;	//increment the number of Functions traced.


	    }			//end outer else

	  break;


	case LB:
	  ++gb;
	  break;

	case RB:
	  --gb;
	  break;

	case END:

	  if (gb < 0)
	    {
	      SET_ERROR (EXTRA_RIGHT_BRACE);
	    }
	  else if (gb > 0)
	    {
	      SET_ERROR (EXTRA_LEFT_BRACE);
	    }

	  err (0, "Function Precursor Returning.");

	  err_type = OK;

	  return num_fun;

	default:

#ifdef DEBUG

	  token_info ()
#endif
	    ;
	  //continue         

	}			//end switch

    }				//end while

  return 0;			//not reached

}				//end routine                   



int
argument_parser ()
{

  Token t;

  extern Token last_token;

  int num = 0;

  while ((t = get_token ()) != RP)
    {

      switch (t)
	{

	case IDENTIFIER:

	  if (num)
	    {

	      if (last_token != COMMA)
		{

		  SET_ERROR (SYNTAX_ERROR);

		}

	    }

	  ++num;

	  break;

	case COMMA:

	  if (last_token != IDENTIFIER)
	    {

	      SET_ERROR (SYNTAX_ERROR);

	    }

	  break;

	default:

	  err_type = SYNTAX_ERROR;

	}			//end switch

      if (err_type != OK)
	{

#ifdef DEBUG

	  err (0, "Argument Parser:Syntax Error at line %d", lines + 1);

	  SET_ERROR (err_type);

#endif

	}

    }				//end while

  if (num)
    {

      if (last_token != IDENTIFIER)
	{

#ifdef DEBUG
	  err (0, "Argument Parser:Syntax Error at Line %d", lines + 1);
#endif

	  SET_ERROR (SYNTAX_ERROR);

	}

    }

#ifdef DEBUG
  err (0, "Total Arguments scanned:%d", num);
#endif

  return num;

}				//end routine


/* Keyword handlers */

/* Return handler*/


struct data *
return_handler (unsigned long lookup_stop, int scope, int state,
		int send_status, int *loop_flag)
{

  extern char *reference;
  char *old = reference;
  struct data *ptr = (struct data *) NULL;

  get_token ();

  if (current_token == NEWLINE || current_token == COMMA)
    {

      SET_ERROR (SYNTAX_ERROR);

    }

  else if (current_token == TERMINATOR)
    {

      if (send_status)
	ptr = zero_object ();	//return a null object

    }

  else
    {

      reference = old;
      if (*reference != ' ')
	{

	  SET_ERROR (SYNTAX_ERROR);
	}

      if (send_status)
	ptr = exp_evaluate ((unsigned long) 0, lookup_stop, scope);

    }

  return ptr;

}				//end routine


struct data *
fun_handler (unsigned long lookup_stop, int scope, int state, int send_status,
	     int *loop_flag)
{

  struct data *dp = (struct data *) NULL;

  if (state)
    {

      SET_ERROR (FUNCTION_WITHIN_FUNCTION);

    }

  while (get_token () != LB)
    ;

  if (!send_status)
    dp = block_parser (lookup_stop, scope, state, send_status, loop_flag);	//syntax checking

  else
    skip_block ();		//skip the block

  return dp;

}


int
do_cond (unsigned long lookup_stop, int scope, int send_status)
{
  extern char *reference;
  char *old;
  struct data *ptr;
  unsigned long stop_addr;

  get_token ();
  if (current_token != LP)
    {
      SET_ERROR (SYNTAX_ERROR);
    }

  old = reference;

  get_stop_addr (&stop_addr);	//get the stop address for the exp_evaluate

  if (!send_status)
    return send_status;

  reference = old;

  ptr = exp_evaluate (stop_addr, lookup_stop, scope);

  return eval_cond (ptr);

}

int
eval_cond (struct data *ptr)
{

  int status = 0;

  if (!ptr)
    {

      SET_ERROR (NULL_DATA);

    }

  switch (ptr->ele_type)
    {

    case _INT:
      status = ptr->INT_VALUE > 0;
      break;

    case _FLT:
      status = ptr->FLT_VALUE > 0;
      break;

    case _DBL:
      status = ptr->DBL_VALUE > 0;
      break;

    default:
      SET_ERROR (IF_ERROR);

    }

  deinstall_data (ptr);

  return status;

}

void
skip_newlines ()
{
  while (get_token () == NEWLINE);
}

/*Get to the start of the block by skipping newlines,if any*/

void
block_start ()
{

  skip_newlines ();

  if (current_token != LB)
    {

      SET_ERROR (SYNTAX_ERROR);
    }

}

/* Get to the end of the block.Needed for breaks.*/

void
skip_block ()
{

  int lb = 1;

  while (lb)
    {
      get_token ();
      if (current_token == LB)
	lb++;
      else if (current_token == RB)
	lb--;
    }

}

/* Skip a single line or statement */

void
skip_statement ()
{

  while (get_token ())
    {

      if (err_type != OK)
	{

	  SET_ERROR (err_type);
	}

      if (current_token == NEWLINE || current_token == END)
	{

	  SET_ERROR (SYNTAX_ERROR);

	}

      if (current_token == TERMINATOR)
	break;

    }

}


/* The if loop handler */

struct data *
if_handler (unsigned long lookup_stop, int scope, int state, int status,
	    int *loop_flag)
{

  int last_status, send_status;

  struct data *ptr = (struct data *) NULL;

  char *old, *p;

  last_status = send_status = do_cond (lookup_stop, scope, status);

  block_start ();		//get to the start of the block

  if (!status)
    block_parser (lookup_stop, scope, state, status, loop_flag);

  else
    {

      if (send_status)
	{
	  ptr =
	    block_parser (lookup_stop, scope, state, send_status, loop_flag);


	  if (ptr || *loop_flag == BREAK || *loop_flag == CONTINUE)
	    return ptr;

	}

      else
	skip_block ();

    }

  while (1)
    {

      old = reference;

      while (get_token () == NEWLINE);

      if (current_token == IDENTIFIER)
	{

	  if (!strcmp (tok, "else"))
	    {

	      p = reference;

	      get_token ();

	      if (!strcmp (tok, "if"))
		{

		  send_status = do_cond (lookup_stop, scope, status);

		  block_start ();	//go to the beginning of the block

		  if (!status)

		    block_parser (lookup_stop, scope, state, 0, loop_flag);

		  else
		    {

		      if (!last_status && send_status)
			{

			  last_status = send_status;	//reset last status 

			  ptr =
			    block_parser (lookup_stop, scope, state,
					  send_status, loop_flag);

			  if (ptr || *loop_flag == BREAK
			      || *loop_flag == CONTINUE)
			    break;


			}

		      else
			skip_block ();

		    }

		}

	      else
		{		//look for else alone

		  reference = p;

		  block_start ();

		  if (!status)

		    block_parser (lookup_stop, scope, state, 0, loop_flag);

		  else
		    {

		      if (!last_status)
			{

			  ptr =
			    block_parser (lookup_stop, scope, state, 1,
					  loop_flag);

			  if (ptr || *loop_flag == BREAK
			      || *loop_flag == CONTINUE)
			    break;

			}

		      else
			skip_block ();

		      break;

		    }


		}

	    }
	  else
	    {

	      reference = old;

	      break;

	    }

	}
      else
	{

	  reference = old;

	  break;
	}

    }

  return ptr;

}

/* Break handler */

struct data *
break_handler (unsigned long lookup_stop, int scope, int state, int status,
	       int *loop_flag)
{

  struct data *ptr = (struct data *) NULL;

  if (status && *loop_flag == OFF)
    {
      SET_ERROR (BREAK_OUTSIDE_LOOP);
    }

  get_token ();

  if (current_token != TERMINATOR)
    {

      SET_ERROR (SYNTAX_ERROR);

    }

  if (status)
    *loop_flag = BREAK;		//set the break flag for active states

  return ptr;

}

/* Continue handler */

struct data *
continue_handler (unsigned long lookup_stop, int scope, int state, int status,
		  int *loop_flag)
{

  struct data *ptr = (struct data *) NULL;

  if (status && *loop_flag == OFF)
    {

      SET_ERROR (CONTINUE_OUTSIDE_LOOP);
    }
  get_token ();
  if (current_token != TERMINATOR)
    {
      SET_ERROR (SYNTAX_ERROR);
    }
  if (status)
    *loop_flag = CONTINUE;
  return ptr;
}

/* The while loop handler */

struct data *
while_handler (unsigned long lookup_stop, int scope, int state, int status,
	       int *lf)
{
  int send_status;
  extern char *reference;
  char *old, *jmp_point;
  int loop_flag = ON;

  struct data *dp = (struct data *) NULL;

  old = reference;		//save the start point

  send_status = do_cond (lookup_stop, scope, status);

  block_start ();

  jmp_point = reference;

  if (!status)
    return block_parser (lookup_stop, scope, state, status, &loop_flag);	//syntax checker


  while (send_status)
    {

      dp = block_parser (lookup_stop, scope, state, send_status, &loop_flag);

      if (dp)
	{

	  return dp;
	}

      if (loop_flag == BREAK)
	break;

      if (loop_flag == CONTINUE)
	loop_flag = ON;

      reference = old;		//reset to test expression

      send_status = do_cond (lookup_stop, scope, status);

      block_start ();

    }

  reference = jmp_point;	//reset to end of block

  skip_block ();

  return dp;

}


void
skip_while (char *old, unsigned long lookup_stop, int scope, int send_status)
{
  extern char *reference;

  if (send_status)
    {
      reference = old;
      skip_block ();
    }

  skip_newlines ();

  if (strcmp (tok, "while"))
    {

      SET_ERROR (DO_WITHOUT_WHILE);
    }

  do_cond (lookup_stop, scope, 0);

  get_token ();

  if (current_token != TERMINATOR)
    {

      SET_ERROR (SYNTAX_ERROR);

    }


}

/*  The do-while loop handler */

struct data *
dowhile_handler (unsigned long lookup_stop, int scope, int state,
		 int send_status, int *lf)
{

  extern char *reference;
  char *old;
  char *jmp_point = (char *) NULL;
  int exp_status;
  int loop_flag = ON;
  struct data *ptr = (struct data *) NULL;

  block_start ();		//get to the start of the block
  old = reference;		//set up the jmp point

  do
    {

      ptr = block_parser (lookup_stop, scope, state, send_status, &loop_flag);
      if (!send_status || ptr || loop_flag == BREAK)
	{

	  if (!jmp_point)
	    skip_while (old, lookup_stop, scope, send_status);

	  break;

	}

      if (loop_flag == CONTINUE)
	{

	  loop_flag = ON;	//reset loop_flag

	  reference = old;

	  skip_block ();
	}

      skip_newlines ();

      exp_status = do_cond (lookup_stop, scope, send_status);

      get_token ();		//eat up semicolon

      if (!jmp_point)

	jmp_point = reference;

      reference = old;

      current_token = LB;

    }
  while (exp_status);

  if (jmp_point)
    reference = jmp_point;

  return ptr;

}


/* For loop initialise and Increment */

void
init_incr (unsigned long lookup_stop, int scope, int send_status, int init)
{

  char l[TOKEN + 1];		//to hold the lval

  char *stop_addr;

  char *old = reference;

  struct data *ptr = (struct data *) NULL;

  int lp = 1;

  while (lp)
    {

      get_token ();

      if (init)
	{

	  if (current_token == TERMINATOR)
	    break;

	}
      else
	{

	  if (current_token == RP)
	    --lp;

	  else if (current_token == LP)
	    ++lp;

	  else if (send_status && current_token == TERMINATOR)
	    {

	      SET_ERROR (SYNTAX_ERROR);

	    }

	}

      if (current_token == NEWLINE || current_token == END)
	{

	  SET_ERROR (SYNTAX_ERROR);

	}


    }

  if (!send_status)
    return;

  stop_addr = reference - 1;

  reference = old;

  get_token ();

  while (reference < stop_addr)
    {

      if (current_token == IDENTIFIER)
	{

	  my_strncpy (l, tok, TOKEN);

	  get_token ();

	  if (current_token != EQUAL)
	    {

	      SET_ERROR (SYNTAX_ERROR);
	    }

	  ptr = exp_evaluate ((unsigned long) stop_addr, lookup_stop, scope);

	  if (!ptr)
	    {

	      SET_ERROR (NULL_DATA);

	    }

	  lval_load (l, ptr);	//load the lval

	  lval_install (lookup_stop, scope);	//install the lval

	  if (reference < stop_addr)

	    get_token ();

	}

      else
	{

	  SET_ERROR (FOR_SYNTAX_ERROR);

	}


    }

  if (reference == stop_addr)
    {

      SET_ERROR (FOR_SYNTAX_ERROR);
    }

}


/* For expression handler */

int
for_expr (char *expr_point, unsigned long lookup_stop, int scope)
{

  struct data *ptr;

  extern char *reference;

  reference = expr_point;

  ptr = exp_evaluate ((unsigned long) 0, lookup_stop, scope);

  return eval_cond (ptr);

}

void
setup_for ()
{

  int lp = 1;

  while (lp)
    {
      get_token ();
      if (current_token == RP)
	--lp;
      else if (current_token == LP)
	++lp;
    }

  block_start ();

}

void
exit_for ()
{

  setup_for ();

  skip_block ();

}

/* The For Loop Handler */

struct data *
for_handler (unsigned long lookup_stop, int scope, int state, int send_status,
	     int *lf)
{
  int loop_flag = ON, exp_status;
  char *expr_point, *incr_point;
  char *jmp_point = (char *) NULL;
  char *start_point = (char *) NULL;
  extern char *reference;
  struct data *ptr = (struct data *) NULL;

  get_token ();
  if (current_token != LP)
    {
      SET_ERROR (SYNTAX_ERROR);
    }

  init_incr (lookup_stop, scope, send_status, send_status ? 1 : 0);	//initialise part of for loop

  if (!send_status)
    {

      block_start ();

      return block_parser (lookup_stop, scope, state, send_status,
			   &loop_flag);

    }

  expr_point = reference;	//save the expression point

  exp_status = for_expr (expr_point, lookup_stop, scope);	//get the expression status

  incr_point = reference;	//save the increment point

  while (exp_status)
    {

      if (!start_point)
	{
	  setup_for ();
	  start_point = reference;
	}

      else
	{

	  reference = start_point;
	  current_token = LB;
	}

      ptr = block_parser (lookup_stop, scope, state, send_status, &loop_flag);

      if (ptr || loop_flag == BREAK)

	break;

      if (loop_flag == CONTINUE)
	loop_flag = ON;

      else if (!jmp_point)
	jmp_point = reference;	//save the jmp point

      reference = incr_point;	//set reference to incr point

      init_incr (lookup_stop, scope, send_status, 0);	//increment

      exp_status = for_expr (expr_point, lookup_stop, scope);

    }

  if (jmp_point)
    reference = jmp_point;

  else if (start_point)
    {
      reference = start_point;
      skip_block ();
    }
  else
    {
      reference = incr_point;
      exit_for ();

    }

  return ptr;

}

struct argument *
builtin_handler (unsigned long lookup_stop, int scope)
{

  struct argument *arg_ptr = arg_node ();
  char *old;
  int args = 0;
  extern char *reference;
  unsigned long stop_addr;

  old = reference;
  get_stop_addr (&stop_addr);


  reference = old;

  /* Initialise the argument pointer */

  arg_initialise (arg_ptr);

  /* Get into a loop and fetch the arguments */

  while (reference < (char *) stop_addr)
    {

      /*Install into the argument structure by evaluating the expression */

      arg_install (arg_ptr, (char *) NULL,
		   exp_evaluate (stop_addr, lookup_stop, scope));
      args++;
    }

  if (!args || reference == (char *) stop_addr)
    {

      SET_ERROR (PRINT_SYNTAX_ERROR);

    }

  return arg_ptr;

}

struct data *
output_handler (unsigned long lookup_stop, int scope, int status, int flag)
{

  struct argument *arg_ptr;

  struct data *ptr = (struct data *) NULL;

  get_token ();

  if (current_token != LP)
    {

      SET_ERROR (SYNTAX_ERROR);
    }


  if (!status)
    {

      unsigned long stop_addr;

      get_stop_addr (&stop_addr);

    }
  else
    {

      arg_ptr = builtin_handler (lookup_stop, scope);

      ptr = my_generic_printf (arg_ptr, flag);

    }


  return ptr;

}


/* The Printf handler */

struct data *
printf_handler (unsigned long lookup_stop, int scope, int status)
{


  return output_handler (lookup_stop, scope, status, 0);

}

/* sprintf Handler : Only Difference with Printf is:
   printf returns the number of bytes written,and sprintf returns the output buffer */

struct data *
sprintf_handler (unsigned long lookup_stop, int scope, int status)
{

  return output_handler (lookup_stop, scope, status, 1);

}


  /* Now start processing of the argument list */

/* Stop address evaluation*/

void
get_stop_addr (unsigned long *stop_addr)
{

  int lp = 1;

  while (lp)
    {

      get_token ();

      if (current_token == LP)
	++lp;

      else if (current_token == RP)
	--lp;

      else if (current_token == TERMINATOR || current_token == NEWLINE
	       || current_token == END)
	{
	  SET_ERROR (SYNTAX_ERROR);
	}

    }				//end while

  *stop_addr = (unsigned long) (reference - 1);

}



/*Return a zero object for null returns*/

struct data *
zero_object ()
{

  struct data *ptr = MALLOC_DATA;

  if (!ptr)
    {

      SET_ERROR (MEM_ERROR);
    }

  ptr->ele_type = _INT;

  return install_data (ptr, "0");
}


/* The block parser:

   Parses a block based on a scope.
  Lookups are performed based on scopes.
  Args: lookups_stop,scopeid and valid return:

*/

/* Send status decides whether to do only syntax checking or both syntax and lval handling.Loop flags decide the fate of loops. */

struct data *
block_parser (unsigned long lookup_stop, int scope, int state,
	      int send_status, int *loop_flag)
{
  /* Parse a block */

  int nb = 0;

  char l[TOKEN + 1];

  char *old;

  struct data *dp;

  struct keywords *k;

  struct intern *builtin;

  if (current_token != LB)
    {

      block_start ();

      if (current_token != LB)
	{
	  SET_ERROR (SYNTAX_ERROR);
	}

    }


  nb++;

  while (nb)
    {				//go inside a block

      old = reference;

      get_token ();

      if (current_token == END)
	{

	  SET_ERROR (SYNTAX_ERROR);

	}

      if (current_token == NEWLINE || current_token == TERMINATOR)
	continue;

      if (current_token == IDENTIFIER)
	{

	  my_strncpy (l, tok, TOKEN);	//try setting up the lval

	  if (!strcmp (l, "else"))
	    {

	      SET_ERROR (ELSE_WITHOUT_IF);

	    }

	  builtin = lookup_intern (l);

	  if (builtin)
	    {

	      dp = builtin->intern_ptr (lookup_stop, scope, send_status);

	      get_token ();

	      if (current_token != TERMINATOR)
		{

		  SET_ERROR (SYNTAX_ERROR);

		}

	      if (dp)

		deinstall_data (dp);

	      continue;

	    }


	  k = lookup_keyword (l);

	  if (k)
	    {

	      dp =
		k->keyword_ptr (lookup_stop, scope, state, send_status,
				loop_flag);

	      if (dp)
		{
		  if (state)
		    return dp;
		  else
		    {
		      SET_ERROR (RETURN_NOT_ALLOWED);
		    }

		}
	      else if (*loop_flag == BREAK || *loop_flag == CONTINUE)
		return dp;

	      continue;

	    }


	  get_token ();

	  if (current_token != EQUAL)
	    {			//then simply call the expression parser

	      reference = old;	//restore the old reference

	      if (send_status)
		{
		  dp = exp_evaluate ((unsigned long) 0, lookup_stop, scope);

		  if (dp)
		    {
#ifdef SHOW_DATA
		      show_data (dp);
#endif

		      deinstall_data (dp);

		    }

		}

	      else
		skip_statement ();	//syntax checker

	    }

	  else
	    {			//the send_status patch

	      if (send_status)
		{
		  dp = exp_evaluate ((unsigned long) 0, lookup_stop, scope);

		  if (dp)
		    {

#ifdef SHOW_DATA
		      show_data (dp);
#endif

		      lval_load (l, dp);

		      lval_install (lookup_stop, scope);

		    }

		}

	      else
		skip_statement ();

	    }


	}

      else if (current_token == RB)

	--nb;

      else if (current_token == LB)

	++nb;

      else
	{

	  reference = old;

	  if (send_status)
	    {

	      dp = exp_evaluate ((unsigned long) 0, lookup_stop, scope);

	      if (dp)
		{

#ifdef SHOW_DATA
		  show_data (dp);
#endif

		  deinstall_data (dp);

		}

	    }

	  else
	    skip_statement ();

	}

    }				//end while



  if (state && *loop_flag == OFF)
    return zero_object ();

  return (struct data *) NULL;

}
