/* The global scope handler */

#ifndef _GLOBAL_H
#define _GLOBAL_H 1

#include <hash.h>
#include <stack.h>
#include <scope.h>
#include <data.h>
#include <error.h>

struct global
{
  struct global *next;
  struct variable *var;
};

/* The extern declaration of the hash table */

extern struct global *global_hash_table[];

extern struct global *global_node ();

/* The install and lookup routines for global scope */

extern void global_push (struct variable *var, unsigned long lookup_stop);

extern struct global *global_trace (char *key, unsigned long lookup_stop);

extern struct data *global_lookup (char *key, unsigned long lookup_stop);

extern void global_destroy ();	//just a single destroy removing all the nodes

extern void global_free ();

#endif
