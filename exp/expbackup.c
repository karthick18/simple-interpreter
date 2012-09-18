#include<stdio.h>
#include<math.h>
#include<setjmp.h>
#include <symbol.h> //call my header

/* An expression parser employing tables to parse the expressions */

/* first define the tokens that you are going to parse */
/* and second define the actions that you are going to take based on the combination of tokens arrived at */

typedef enum{
false,
true,
}bool;


/*the tokens that will be parsed */

/*take a simple case */

/*parse '+', '-', '*', '/' */

typedef enum{

PLUS,

MINUS,

MULTIPLY,

DIVIDE,

L,

R,

END,

MAX, //max tokens

VALUE, //identifies a double constant

IDENTIFIER, //for a double

EQUAL, //for equal

RANDOM,

}Token;

bool firsttok;
Token prevtok;

/* This defines the token actions */

typedef enum{

REDUCE, /* implies that you are evaluating the value*/

SHIFT , /*implies that you are proceeding further with parsing */

TERMINATE, /*implies that you are stopping the whole action*/

E1, /* missing left parenthesis */

E2, /*missing right parenthesis */

E3, /*extra left parenthesis */


}TokenAction; //actions to be token on specific combinations


/*Define a value stack to hold the value */
/*But before that,Define the top of the stack to hold 100 elements*/


typedef struct val {

#define TOP 100 

double value[TOP]; //the stack

int top; //the top of the value stack

struct value_stack_operations *vptr; //pointer to the stack operation structure

}ValueStack; 


/* Define an operator stack for holding the operators */


typedef struct op {

char operator[TOP]; //the operator stack

int top; //the top of the operator stack

struct operator_stack_operations *optr; //pointer to the operation structure

}OperatorStack;



/*Now define the parser table,with the actions mentioned */



char ParserTable[MAX][MAX]= {
              /* + */  /* - */ /* * */ /* / */      /* end */

/* + */   {REDUCE,REDUCE,SHIFT, SHIFT, SHIFT,REDUCE,REDUCE  },	

/* - */   {REDUCE,REDUCE,SHIFT, SHIFT,SHIFT,REDUCE,REDUCE   },	

/* * */   {REDUCE,REDUCE,REDUCE, REDUCE,SHIFT,REDUCE,REDUCE },	

/* / */   {REDUCE,REDUCE,REDUCE, REDUCE,SHIFT,REDUCE,REDUCE },	
/* (*/          {SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,E2},
/*)*/       {REDUCE,REDUCE,REDUCE,REDUCE,E3,REDUCE,REDUCE} , 
/*end*/ {SHIFT, SHIFT,SHIFT,SHIFT, SHIFT,E1,TERMINATE},


}; /* END OF ACTION TABLE DEFINITION */

	

/*Now define the basic global variables that the Exp. Parser depends on */

Token tok; //this is a global token type holder


/* Now define an operation structure for the 2 stacks.Thats a basic push and a pop routine */

typedef struct value_stack_operations {

void    (*push)(double,ValueStack*); //push a double into the stack

double  (*pop) (ValueStack*)  ; //pop a double from the stack

}ValueStackOperation;


/* For the operator stack */

typedef struct operator_stack_operations {

void   (*push)(char,OperatorStack *); //push an operator,thats a character into the stack

char   (*pop) (OperatorStack*); //pop the operator from the stack


}OperatorStackOperation;


void ValuePush(double,ValueStack *);

double ValuePop(ValueStack *);

void OperatorPush(char,OperatorStack *);
char OperatorPop(OperatorStack *);

/* define the operation structure for the 2 stacks */


static ValueStackOperation vop = {

ValuePush,

ValuePop,

};

static OperatorStackOperation oop = {

OperatorPush,

OperatorPop,

};



/*global buffer*/

char *buffer=NULL; //global buffer to store the input

char *bufferptr; 

char value[10]; //a string to hold the value


jmp_buf env; //for set and long jump


char *SkipSpace(char*);

double Atof(char*);

/* Step 1,get the input from the user,till he enters quit */


#define BUFFER 100
char identifier[MAX],key[MAX];
GetInput(){ //get the input from the user

unsigned int bytes;

//get the input from the user into the buffer 
while ( 1 ) {

setjmp(env); 

if(buffer) free(buffer);

buffer=(char*)malloc(BUFFER); //allocate from the heap

printf("Enter expression to evaluate.Enter \"quit\" to exit:\n");
if(fgets(buffer,BUFFER,stdin)==NULL) 
return 0;

bytes=strlen(buffer);

if(buffer[bytes-1] == '\n' ) //strip newline

{

--bytes;

buffer[bytes] = '\0';

}

if(strstr(buffer,"quit") ) return 0;

bufferptr=buffer; //make it point to the beginning of the buffer
tok=RANDOM;
firsttok=true;
identifier[0]='\0';
key[0]='\0';

Driver(); //drive the buffer containing the expression

} //end while


} //end GetInput


/*next important routine*/

/*Parse buffer */
/*have a global string to indicate the identifier on the left side of expression */
GetToken() {

char *hold=value;
prevtok=tok;

if(isspace(*bufferptr) || *bufferptr == '\t')

bufferptr=SkipSpace(bufferptr); //skip spaces in the buffer 


switch(*bufferptr) {

case '+':
            
	tok=PLUS;
	break; 

case '-':
	tok=MINUS;
	break;

case '*':
	tok=MULTIPLY;
	break;

case '/':
	tok=DIVIDE;
	break;

case '=':
 	if(prevtok!=IDENTIFIER)
        {
        fprintf(stderr,"The last token has to be an identifier:\n");
        longjmp(env,0);
        }
        tok=EQUAL;
        break;

case '(':
        tok=L;
        break;
 
case ')':
        tok=R;
        break;

case '\0':
	tok=END;
	break;

default:

if(isdigit(*bufferptr)) {
for(; isdigit(*bufferptr) ; bufferptr++)

*hold++=*bufferptr;

if(*bufferptr=='.') { //take the decimal

*hold++=*bufferptr++;

for( ; isdigit(*bufferptr); bufferptr++)

*hold++=*bufferptr;


} //end if

*hold='\0';
 
tok=VALUE;

} //end outer if

else if(isalnum(*bufferptr)){ //parse the identifier portion of the expression
if(prevtok==RANDOM) { //implies its the first token
char *ident=identifier;  
while(isalnum(*bufferptr))
*ident++=*bufferptr++;
*ident='\0'; //null terminate the first token

}
else {
char *k=key;
while(isalnum(*bufferptr)){
*k++=*bufferptr++;
}
*k='\0'; //null terminate the key
} //end else
tok=IDENTIFIER;

} //end if for 'isalnum'

else {

fprintf(stderr,"Error in token:\n");
longjmp(env,0); //jump to a safe point

}


} //end switch

if(tok!=VALUE && tok!=IDENTIFIER) bufferptr++;


} //end routine


/* Stack operations for operator and value */


void OperatorPush(char c,OperatorStack *o){ //push the operator into the stack


if(o->top > MAX )
{
fprintf(stderr,"Operator Stack overflow:\n");
longjmp(env,0);
}

o->operator[o->top++]=c;

} //end routine


char OperatorPop(OperatorStack *o){


if(o->top == 0 ) {

fprintf(stderr,"Operator Stack underflow:\n");
longjmp(env,0);

}

return o->operator[--o->top];

} //end PopOperation


void ValuePush(double val,ValueStack *v){

if(v->top > MAX ){
fprintf(stderr,"Value stack overflow:\n");
longjmp(env,0);
}

v->value[v->top++]=val;

}


double ValuePop(ValueStack *v){

if(v->top == 0 ){
fprintf(stderr,"Value Stack underflow:\n");
longjmp(env,0);
}

return v->value[--v->top];

}

InitialiseStack(OperatorStack *o,ValueStack *v){

o->top=0;
v->top=0;

/* initialise the operation pointer */

o->optr=&oop;
v->vptr=&vop; 

/*Despatch the push operation for the operator stack */

o->optr->push((char)END,o); //push END into the operator stack

} 


Driver(){

OperatorStack operatorstack;
ValueStack valuestack;
int top;

InitialiseStack(&operatorstack,&valuestack);


GetToken(); //get the token from the buffer

while ( 1 ) {


//get the first token

if(tok==VALUE) { 


ShiftOperation(&operatorstack,&valuestack); //call shift operation which pushes the token into the stack and gets the next token


continue;
}

if(tok==IDENTIFIER){
if(firsttok)
GetToken(); //get the next token
if(firsttok && tok!=EQUAL)
{
fprintf(stderr,"Error in left side of an expression:\n");
longjmp(env,0);
}
else if(firsttok)
GetToken(); //eat up the equal
firsttok=false;
ShiftOperation(&operatorstack,&valuestack);
continue;
} 

top=operatorstack.top - 1;

//Act on the basis of the operator

switch(

ParserTable[operatorstack.operator[top]][tok] ) {


case REDUCE: 

 //call Reduce operation

ReduceOperation(&operatorstack,&valuestack);

break;


case SHIFT:

//call shift operation
ShiftOperation(&operatorstack,&valuestack);

break;


case TERMINATE:
if(identifier[0]){ //if the identifier is set
Install(identifier,valuestack.value[0]);
}
  if(valuestack.top == 1 ){
fprintf(stderr,"The output is %.2f\n",valuestack.value[0]);
return 0;
}

else 
{
fprintf(stderr,"Error in Evaluation:");
longjmp(env,0);

}
break;

case E1: //missing left parenthesis

fprintf(stderr,"Error:Missing left parenthesis:\n");
longjmp(env,0);

case E2:
fprintf(stderr,"Error missing right parenthesis:\n");
longjmp(env,0);

case E3:
fprintf(stderr,"Error:Extra left parenthesis:\n");
longjmp(env,0);

}//switch


} //end while

} //end routine


ShiftOperation(OperatorStack*o,ValueStack*v){

if(tok==VALUE){
v->vptr->push(Atof(value),v);
} 
else if(tok==IDENTIFIER) { //then load the value from the symbol table
Symbol *s; 
if(!key[0]){
fprintf(stderr,"Error in collecting key:\n");
longjmp(env,0); //jump to a safe point
} 
s=Lookup(key) ;
if(!s) {
fprintf(stderr,"The particular symbol has no definition:\n");
longjmp(env,0);
}
//else key found,push the keys value into the value stack
v->vptr->push(s->value,v);
} //end else for IDENTIFIER

else { //push into the operator stack
o->optr->push((char)tok,o); 
}

//get the next token from the buffer

GetToken();

} //end Shift Operation


ReduceOperation(OperatorStack*o,ValueStack*v){ 

int otop; //operator stack top
int vtop; //value stack top


otop=o->top - 1;
vtop=v->top;
//now check the operator stack and do the corresponding operation


switch(o->operator[otop]){


case PLUS:
if(vtop>=2) {
v->value[vtop-2] = v->value[vtop-2] + v->value[vtop-1];

//pop up the value stack

v->vptr->pop(v); //decrements the top of the stack
}
else 
error();
 
break;


case MINUS: 
if(vtop>=2) { 
v->value[vtop-2] = v->value[vtop-2] - v->value[vtop-1];

v->vptr->pop(v); 
}
else 
error();

break;


case MULTIPLY:
if(vtop>=2){
v->value[vtop-2]=v->value[vtop-2] * v->value[vtop-1];
v->vptr->pop(v);
}
else 
error();

break;


case DIVIDE:
if(vtop>=2){

double check; //for checking the division status

check = v->value[vtop-1];

if(check==(double)0)
{
fprintf(stderr,"Division by zero:\n");
longjmp(env,0);
}

v->value[vtop-2]=v->value[vtop-2] / check ;

v->vptr->pop(v);

}
else
error();

break;

case R: //operator stack contains a right parenthesis

o->optr->pop(o); //pop up the stack
break;

} //end switch

//pop up the operator stack

o->optr->pop(o); 


} //return 


char *SkipSpace(char *ptr){


while(isspace(*ptr) || *ptr == '\t') ptr++;

return ptr;
}


/*the main */

main(){

GetInput(); //get the input from the user

exit(0);


}


double Atof(char *str){

//converts a string to double


double power=1,val;

for(val=0.0;isdigit(*str);str++)
val=val*10.0 + *str - '0';

if(*str=='.'){

str++;

for(power=1.0;isdigit(*str);str++){

val=val*10 + *str - '0';

power*=10;
}

}

return val/power;

}

error(){
fprintf(stderr,"Missing operands in an expression:\n");
longjmp(env,0);
} //end routine

