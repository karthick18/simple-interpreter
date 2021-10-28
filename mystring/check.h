#include<stdio.h>
#include<string.h>

void
str_replace (char *src, char *what, char *by)
{

  char *ptr;

  while ((ptr = strstr (src, what)))
    {

      memmove (ptr + strlen (by), ptr + strlen (what),
	       strlen (ptr + strlen (what)) + 1);
      memcpy (ptr, by, strlen (by));

    }

}
