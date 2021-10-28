/*The gettoken routine that identifies and returns the current token*/

#include<stdio.h>
#include<token.h>
#include<error.h>
#include<string.h>
#include<ctype.h>
#include<utils.h>

char *token_classes[] = {
  "LOGICAL NOT", "ONES COMPLEMENT", "MULTIPLY", "DIVIDE", "PLUS", "MINUS",
    "RIGHT SHIFT", "LEFT SHIFT", "GREATER THAN", "LESS THAN",
    "GREATER THAN OR EQUAL TO", "LESS THAN OR EQUAL TO", "EQUAL EQUAL",
    "NOT EQUAL", "BITWISE AND", "BITWISE XOR", "BITWISE OR", "LOGICAL AND",
    "LOGICAL OR", "LEFT PARENTHESIS", "RIGHT PARENTHESIS", "END", "EQUAL",
    "MAXIMUM OPERATOR COUNT", "SEMI_COLON", "LEFT BRACE", "RIGHT BRACE",
    "COMMA", "DOT", "HASH", "BACKSLASH", "KEYWORDS", "IDENTIFIER",
    "CONSTANT_INT", "CONSTANT_FLOAT", "CONSTANT_STRING", "NEWLINE", "UNKNOWN",
    "SPECIAL", "REDUCE", "SHIFT", "EVALUATE", "MISSING LEFT PARENTHESIS",
    "MISSING RIGHT PARENTHESIS", "EXTRA LEFT PARENTHESIS",
    "ILLEGAL COMPARISON OPERATOR COMBINATION IN EXPRESSION"
};


extern char *reference;

Token current_token = END;	//set up the initial token

Token last_token = END;		//initialise the last token

char allowed[] = "_";

char quote_allowed[] = "_";

char not_allowed[] = "*/\n/\\;!><=&^|,";

char ctrl_char[] = "abnrtv";

char sub_ctrl[] = "\a\b\n\r\t\v";

char tok[TOKEN + 1];

static char *skip_atoi (char *, char **, int);

int
get_token ()
{

  unsigned int count = TOKEN;

  extern Error err_type;

  extern int lines, unknown;

  char *s = tok;		//have a pointer to the tok

  char *check;			//check for unhandled tokens.

  last_token = current_token;	//store the last value of the token

  while (*reference && *reference != '\n' && isspace (*reference))
    reference++;		//skip spaces

  if (!*reference)
    return current_token = END;

  check = reference;

  switch (*reference)
    {


    case '!':

      current_token = LOGICAL_NOT;
      *s++ = *reference++;
      if (*reference == '=')
	{
	  *s++ = *reference++;
	  current_token = NE;
	}

      break;

    case '~':

      *s++ = *reference++;

      current_token = ONES_COMPLEMENT;

      break;

    case '*':

      *s++ = *reference++;

      current_token = MULTIPLY;

      break;

    case '/':

      *s++ = *reference++;

      current_token = DIVIDE;

      break;

      /* Patched for handling signed quantities */

    case '+':

      handle_sign ();

      *s++ = (current_token == PLUS) ? '+' : '-';

      break;

    case '-':

      handle_sign ();

      *s++ = (current_token == MINUS) ? '-' : '+';

      break;

    case '>':

      *s++ = *reference++;

      current_token = GT;

      if (*reference == '=')
	{
	  *s++ = *reference++;
	  current_token = GTE;
	}

      else if (*reference == '>')
	{
	  *s++ = *reference++;
	  current_token = RS;
	}

      break;

    case '<':

      *s++ = *reference++;

      current_token = LT;

      if (*reference == '=')
	{
	  *s++ = *reference++;
	  current_token = LTE;
	}

      else if (*reference == '<')
	{
	  *s++ = *reference++;
	  current_token = LS;
	}


      break;

    case '&':

      current_token = BITWISE_AND;
      *s++ = *reference++;
      if (*reference == '&')
	{
	  *s++ = *reference++;
	  current_token = LOGICAL_AND;
	}
      break;

    case '^':

      current_token = BITWISE_XOR;

      *s++ = *reference++;

      break;

    case '|':

      current_token = BITWISE_OR;

      *s++ = *reference++;

      if (*reference == '|')
	{
	  *s++ = *reference++;
	  current_token = LOGICAL_OR;
	}

      break;

    case '(':

      *s++ = *reference++;

      current_token = LP;

      break;

    case ')':

      *s++ = *reference++;

      current_token = RP;

      break;

    case '{':

      *s++ = *reference++;

      current_token = LB;

      break;

    case '}':

      *s++ = *reference++;

      current_token = RB;

      break;

    case '=':

      *s++ = *reference++;

      current_token = EQUAL;

      if (*reference == '=')
	{
	  *s++ = *reference++;
	  current_token = EE;
	}

      break;

    case ',':

      *s++ = *reference++;

      current_token = COMMA;

      break;

    case '.':

      *s++ = *reference++;

      current_token = DOT;

      break;

    case '#':

      *s++ = *reference++;

      current_token = HASH;

      break;

    case '\\':

      *s++ = *reference++;

      current_token = BACKSLASH;

      break;


    case ';':

      *s++ = *reference++;

      current_token = TERMINATOR;

      break;

    case '\n':

      *s++ = *reference++;

      current_token = NEWLINE;

      lines++;

      break;

    case DOUBLE_QUOTES:	//double quotes

    case SINGLE_QUOTES:	//single doutes

      {

	char compare;
	char *old;
	char *p;

      repeat:

	compare = *reference;	//store the character for future comparison

	++reference;

	while (count-- > 0 && *reference && *reference != compare)
	  {

	    if (*reference == '\\')
	      {

		++reference;

		if (!*reference)
		  break;

		if ((p = strchr (ctrl_char, *reference)))
		  {
		    *s++ = sub_ctrl[p - ctrl_char];	//substitute for ctrl characters
		    reference++;
		  }
		else
		  *s++ = *reference++;

		continue;

	      }

	    else if (*reference == '\n')
	      break;

	    *s++ = *reference++;

	  }

	if (*reference != compare)

	  {

	    if (compare == DOUBLE_QUOTES)


	      err_type = MISSING_DOUBLE_QUOTES;


	    else

	      err_type = MISSING_SINGLE_QUOTES;

	  }

	else

	  {

	    ++reference;

	    old = reference;

	    /* Try hunting for double quote pairs */

	    while (*reference != '\n' && isspace (*reference))
	      reference++;

	    if (*reference == SINGLE_QUOTES || *reference == DOUBLE_QUOTES)
	      goto repeat;

	    else
	      reference = old;


	  }

	current_token = CONSTANT_STRING;

	break;

      }

    default:


      if (isdigit (*reference))
	{
	  s = skip_atoi (s, &reference, count);	//store the numeric part
	}
      else
	{

	  while (count-- > 0 && *reference
		 && (isalnum (*reference) || strchr (allowed, *reference)))

	    *s++ = *reference++;

	  current_token = IDENTIFIER;

	}



    }				//end switch


  if (reference == check)
    {

      current_token = UNKNOWN;

      unknown++;

      *s++ = *reference++;

    }


  *s = '\0';

  return current_token;

}


/* Skip the integers by storing them into the dst: Base 10,8 and 16 supported */

static char *
skip_atoi (char *dst, char **src, int count)
{

  int base = 10;		//set the default base

  int value;


  while (**src != '\n' && isspace (**src))
    (*src)++;


  if (**src == '\n')
    {

      SET_ERROR (SYNTAX_ERROR);
    }

  if (count > 0 && **src == '0' && ((*src)[1] != '.'))
    {

      base = 8;

      *dst++ = *((*src)++);

      --count;

      if (count > 0 && tolower (**src) == 'x' && isxdigit ((*src)[1]))
	{
	  *dst++ = *((*src)++);
	  base = 16;

	  --count;

	}
      else if (count > 0 && tolower (**src) == 'b' && isdigit ((*src)[1]))
	{
	  *dst++ = *((*src)++);
	  base = 2;
	}

    }
  while (count > 0 && isxdigit (**src))
    {

      value =
	isdigit (**src) ? **src -
	'0' : (islower (**src) ? toupper (**src) : **src) - 'A' + 10;

      if (value >= base)
	{

	  err (0, "Base=%d,Value=%d\n", base, value);

	  SET_ERROR (BAD_NUMERIC_CONSTANT);

	}

      *dst++ = *((*src)++);

      count--;

    }

  current_token = CONSTANT_INT;

  if (**src == '.')
    {				//check for decimals

      int c = MAX_DECIMAL;

      if (base != 10)
	{			//decimals only for base 10

	  SET_ERROR (BAD_NUMERIC_CONSTANT);

	}

      else
	{

	  *dst++ = *((*src)++);

	  while (count-- > 0 && c-- > 0 && isdigit (**src))
	    *dst++ = *((*src)++);	//copy into destination   

	  if (c == MAX_DECIMAL - 1)
	    {

	      SET_ERROR (BAD_NUMERIC_CONSTANT);

	    }

	  current_token = CONSTANT_FLOAT;

	}

    }

  return dst;

}				//end routine


int
push_back (int n)
{

  while (n-- > 0)
    --reference;

  return 0;
}

int
token_info (void)
{

  fprintf (stderr,
	   "Current Token Value=%s,Last Token = %s,Current Token = %s\n", tok,
	   token_classes[(int) last_token],
	   token_classes[(int) current_token]);

  return (int) current_token;

}


void
handle_sign ()
{

  extern char *reference;
  int sign = 1;

  while (1)
    {

      while (*reference != '\n' && isspace (*reference))
	reference++;

      if (*reference == '-' || *reference == '+')

	{

	  if (*reference == '-')
	    sign = -sign;

	  ++reference;
	}

      else if (strchr (not_allowed, *reference))
	{

	  SET_ERROR (INVALID_EXPRESSION);

	}

      else
	{

	  if (sign < 0)
	    current_token = MINUS;

	  else
	    current_token = PLUS;

	  break;
	}

    }


}
