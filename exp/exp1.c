/* The expression parser:
 The Expression parser uses a Parser table,which denotes the action to take for the token in the operator stack and the current token.
*/

#include <stdio.h>
#include <exp_val.h>
#include <exp_op.h>
#include <data.h>
#include <stack.h>
#include <token.h>
#include <error.h>
#include <exp.h>
#include <stdlib.h>

char action_table[MAX_OP-1][MAX_OP] = { 
  
  {SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL},

  {SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL,SPECIAL},
  /* * */
  {SPECIAL,SPECIAL,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },
  /* / */
  {SPECIAL,SPECIAL,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },
  /* + */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE},
  /* - */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE},
  /* >> */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE},
  /* << */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE},
  /* > */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,E4,E4,E4,E4,E4,E4,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },
  /* < */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,E4,E4,E4,E4,E4,E4,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },
  /* >= */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,E4,E4,E4,E4,E4,E4,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },
  /* <= */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,E4,E4,E4,E4,E4,E4,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },
  /* == */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,E4,E4,E4,E4,E4,E4,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },
  /* != */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,E4,E4,E4,E4,E4,E4,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },
  /* & */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },  
  /* ^ */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },  
  /* | */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,SHIFT,SHIFT,REDUCE,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },  
  /* && */
  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,REDUCE,REDUCE,SHIFT,REDUCE,REDUCE },  
 
  /* || */

  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,REDUCE,SHIFT,REDUCE,REDUCE },

  /* lp */

  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,E2 },

  /* rp */

  {SPECIAL,SPECIAL,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,REDUCE,E3,REDUCE,REDUCE},

  /*end*/

  {SPECIAL,SPECIAL,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,E1,EVALUATE},

};


void exp_initialise(){
  val_initialise();
  op_initialise();
}

void exp_destroy() {
 
  val_destroy();
  op_destroy();

}

void exp_free() {
  val_free();
  op_free();
}


/*Return a data type pointer by loading the ele_type*/

struct data *get_data(int tok){

  struct data *ptr = MALLOC_DATA;
  if(!ptr){ 

    SET_ERROR(MEM_ERROR);
  }

  ptr->ele_type = (DataType)tok;
  return ptr;

}

/*Evaluate the expression*/

struct data *evaluate() { 
 
  struct data *ptr;
  val_pop(&ptr); //pop from the value stack
  if(!ISVAL_EMPTY && !ISOP_EMPTY )
    {
      exp_free();

      //exp_destroy();

      SET_ERROR(INVALID_EXPRESSION);

    }

  exp_free();
  
  return ptr;
} 

struct data *handle_identifier(char *tok,int action) {

  struct data *ptr = stack_lookup(tok);
  if(! ptr) {
    SET_ERROR(VARIABLE_NOT_FOUND);
  }

  return operate_data(ptr,(struct data *)NULL,action);

}


void exp_error(int action) {

  extern char *token_classes[];

  fprintf(stderr,"%s\n",token_classes[action] );
  
  exp_free();
  exp_destroy();
  exit(1);

}
   
void exp_shift(int tok) {

  /* Push the token into the Operator stack */

  op_push((char)tok);

}

void exp_reduce(int ct){

  /*Reduce the expression,and push the result into the value stack */

  struct data *a;
  struct data *b;
   
  int f1,f2;

  int t;

  t = (int)op_pop(); //pop up the operator stack 

  if (t == RP) 

    op_pop();

  else {

  f1=val_pop(&b);
  f2 =val_pop(&a);
  val_push(operate_data(a,b,t),1); //just call operate data irrespective of the operation to be performed. The value can be freed

  if(f1) {

  #ifdef FREE

    err(0,"Deinstalling data:\n");

  #endif   

 deinstall_data(a);

  }

  if(f2) {

  #ifdef FREE

    err(0,"Deinstalling Data:\n");

  #endif

  deinstall_data(b); 

  }


  }

  if(ct != END)
    
     op_push((char)ct);   

    
} //end reduce    

/*Evaluate an expression*/
/*The expression parser code*/

struct data* exp_evaluate() {
  int ct=SPECIAL;
  extern char tok[];
  struct data *ptr_1; 
  struct data *ptr_2;
  
  op_push((char)END); //push the END into the OP stack

  while(1) {

    if (ct != END)
    ct = get_token();
  
    //token_info();

    if(ct == COMMA || ct == TERMINATOR || ct == NEWLINE) 
      ct = END; //reset everything to end
  
       
    if(ct == LOGICAL_NOT || ct == ONES_COMPLEMENT) {
      
      int action = ct;

      ct = get_token();

      
      if(ct == IDENTIFIER || ct == CONSTANT_INT || ct == CONSTANT_FLOAT) {

        if(ct == IDENTIFIER) 
            
	  {

            ptr_2 = handle_identifier(tok,action); 
                   
          }
       
        else if(ct == CONSTANT_INT){

	  ptr_1 = install_data(get_data((int)_INT),tok);

          ptr_2 = operate_data(ptr_1,(struct data *)NULL,action);        
                    
          free((void*)ptr_1); 

	}

        else {

          ptr_1 = install_data(get_data((int)_FLT),tok);
          ptr_2 = operate_data(ptr_1,(struct data*)NULL,action);   
        
          free((void*)ptr_1);
 
	}

	val_push(ptr_2,1); //push into the value stack

        continue;

      } 

      else { 

	SET_ERROR(INVALID_EXPRESSION);

      }

    }

    else { 

      DataType dt;
      int action;

      switch(ct) { 

      case IDENTIFIER:
        ptr_1 = stack_lookup(tok);
        if(! ptr_1) {

          SET_ERROR(VARIABLE_NOT_FOUND);

	}

        val_push(ptr_1,0); //push into the stack

	break;

      case CONSTANT_INT:
      case CONSTANT_FLOAT:
      case CONSTANT_STRING:

        if(ct == CONSTANT_INT) 
	  dt = _INT;
        else if(ct == CONSTANT_FLOAT) 
          dt = _FLT;
        else 
          dt = _STR; 

        ptr_1 = install_data(get_data((int)dt),tok);                

       #ifdef DEBUG
        err(0,"Pushing %s into the value stack:\n",tok);
       #endif
        val_push(ptr_1,1); //push into the stack

        break;

      default : //an operator

       if(ct >= MAX_OP - 1) {

      SET_ERROR(INVALID_EXPRESSION);
      
      return evaluate();
       
         }
     
        action = (int)action_table[(int)og_ptr->tail->op][ct];
         
	switch(action) {

	case SHIFT:

        #ifdef DEBUG
          err(0,"Shift operation:\n");
        #endif
	  exp_shift(ct);

          break;

        case REDUCE:
        #ifdef DEBUG
          err(0,"Reduce Operation:\n");
        #endif
          exp_reduce(ct);

          break;

        case EVALUATE:
        #ifdef DEBUG
          err(0,"Evaluation:\n");
        #endif
          return evaluate();
                
	default : 

	  exp_error(action);

        }

      }


    }

  }

}








