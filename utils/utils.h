#ifndef _UTILS_H
#define _UTILS_H
#define BUFFER 1024


extern char *initialise (int argc, char **argv);

extern char *read_file (char *filename);

extern void err (int, char *fmt, ...);

extern void my_strncpy (char *dst, char *src, unsigned int nbytes);

extern int function_precursor (void);

extern int argument_parser (void);

extern int lines, unknown;

extern struct data *block_parser (unsigned long lookup_stop, int scope, int r,
				  int send_status, int *loop_flag);

extern void get_stop_addr (unsigned long *stop_addr);

extern struct data *zero_object (void);

int do_cond (unsigned long lookup_stop, int scope, int status);

void block_start (void);

void skip_block (void);

void skip_newlines (void);

void skip_statement (void);

void skip_while (char *old, unsigned long lookup_stop, int scope,
		 int send_status);

void init_incr (unsigned long lookup_stop, int scope, int send_status,
		int init);

int eval_cond (struct data *ptr);

int for_expr (char *expr_point, unsigned long lookup_stop, int scope);

void setup_for (void);

void exit_for (void);

int my_atoi (char *s);

int determine_base (char **s);


#endif
