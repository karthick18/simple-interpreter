/*Expression header*/
#ifndef _EXP_H

#define _EXP_H 1

#include <token.h>
#include <exp_op.h>
#include <exp_val.h>

extern char action_table[MAX_OP-1][MAX_OP];

extern void exp_destroy(struct value_stack *vg_ptr,struct operator_stack *og_ptr);
extern void exp_error(struct value_stack *vg_ptr,struct operator_stack *og_ptr,int action);
extern struct data *get_data(int dt);
extern struct data *evaluate(struct value_stack *vg_ptr,struct operator_stack *og_ptr);
extern struct data *exp_evaluate(unsigned long stop_addr,unsigned long lookup_stop,int scope);

void exp_shift(struct operator_stack *og_ptr,int ct);
void exp_reduce(struct value_stack *vg_ptr,struct operator_stack *og_ptr,int ct);
struct data *handle_identifier(int sign,char *tok,int action,unsigned long lookup_stop,int scope);

#endif

