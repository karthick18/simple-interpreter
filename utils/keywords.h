
#ifndef _KEYWORDS_H
#define _KEYWORDS_H 1

#define KEYWORDS 10

#define OFF 0
#define ON  1
#define BREAK 2
#define CONTINUE 3

#include <data.h>

/*Define the keyword structure*/

struct keywords {

  char *keyword;
  struct data * (*keyword_ptr)(unsigned long,int,int,int,int *);

};

extern struct data * return_handler(unsigned long lookup_stop,int scope,int state,int send_status,int *loop_flag);
extern struct data * while_handler(unsigned long lookup_stop,int scope,int state,int send_status,int *loop_flag);
extern struct data * if_handler(unsigned long lookup_stop,int scope,int state,int send_status,int *loop_flag);
extern struct data *fun_handler(unsigned long lookup_stop,int scope,int state,int send_status,int *loop_flag);
extern struct data *break_handler(unsigned long lookup_stop,int scope,int state,int send_status,int *loop_flag);
extern struct data *continue_handler(unsigned long lookup_stop,int scope,int state,int send_status,int *loop_flag);
extern struct data *dowhile_handler(unsigned long lookup_stop,int scope,int state,int send_status,int *loop_flag);
extern struct data *for_handler(unsigned long lookup_stop,int scope,int state,int send_status,int *loop_flag);
extern struct keywords *lookup_keyword(char *keyword);

#endif





