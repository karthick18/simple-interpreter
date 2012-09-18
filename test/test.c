#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define DATA 10
#define _INT 0

struct test{
  void *ptr;
  struct test *next;
};

struct object_operations {

  int (*add_op) (int a,int b);

  int (*sub_op) (int a,int b);

};


struct data_operations {

  struct object_operations *op_ptr;

}data_operations[DATA];


struct data{
  float *ptr;

  union elements {

    int a;

    char c[2];

  }elements;

  struct object_operations *op_ptr;

}fl;


int add_op(int a,int b);
int sub_op(int a,int b);
int perform_operation(int a,int b,int op);
int initialise_data(void);

static struct object_operations operations = {

  add_op,
  NULL,

    };


extern int  test_operate();

struct test *traverse;

int main(int argc,char *argv[]){

  struct test copy;

  struct data * db;

  char buffer[10];
  char *c;

  signed  int k=32769;

  unsigned long *ptr;
  unsigned long a,b;

  initialise_data();

  db = &fl;
  
  ptr = &a;

  fl.ptr = (float *) malloc(sizeof(float) );

  * (fl.ptr) =   3.5;

  a=6;
  b=5;

  traverse = (struct test *) malloc(sizeof (*traverse) );

  traverse->ptr = (void *) malloc(DATA+1); 

  strcpy((char*)traverse->ptr,"karthick");

  printf("%s\n",(char*)traverse->ptr);

  c = memccpy((char *)buffer,(char*)traverse->ptr,'t',DATA);
  
  *c = 0;

  printf("CH:%s\n",buffer);
  
  bzero((char*)traverse->ptr,strlen((char*)traverse->ptr) );

  traverse->ptr = (void*)ptr;
 
  printf("%d\n",*( (unsigned long*) traverse->ptr ) );
 
  // make a copy of the structure traverse

  copy = *traverse;
  
  printf("%d\n", *( (unsigned long *)copy.ptr) );
  
  copy.ptr = (void*) &b;

  printf("Copy:%d\n", *( (unsigned long *)copy.ptr) );

  printf("Traverse:%d\n", *( (unsigned long *)traverse->ptr) );

  traverse->ptr = (char*) "radha";

  printf("String:%s\n", (char *)traverse->ptr );

  printf("K=%d\n",k);
  printf("F=%.2f\n",*(fl.ptr));

  fl.elements.a = 10;

  printf("U:%d\n",fl.elements.a);

  printf("U2:%d\n",db->elements.a);

  test_operate();
  
  db->op_ptr = &operations;

  if(db->op_ptr->sub_op)  
  printf("Op:%d\n",db->op_ptr->sub_op(1,3) );
  else 
    printf("Not found:\n");

  if ( register_data(&operations,_INT) < 0 ) 

    exit(1);

  { int status;  
  
 status =   perform_operation(3,5,'+');

 printf("Performed:%d\n",status);
  }
  
}

int initialise_data(){

  int i;
 
 struct data_operations *dptr;

  for (i=0;i < DATA;i++) {
     
    dptr = &data_operations[i];

  dptr->op_ptr = (struct object_operations *) NULL;

  }

  return 0;

}

int register_data(struct object_operations *ptr,int dt){

  struct data_operations *dptr;

  if (dt >= DATA) {
 
    fprintf(stderr,"Error:");

    return -1;
  }

  dptr = &data_operations[dt];

  dptr->op_ptr = ptr ;

  return 0;

}
  

 int perform_operation(int a,int b,int op){

   struct data_operations *dptr;

    switch(op){

   case '+':

     dptr = &data_operations[_INT];

     if (dptr->op_ptr->add_op) 

       return dptr->op_ptr->add_op(a,b);
     
     fprintf(stderr,"Error:Data type not registerd:\n");

     exit(1);

   default:

     fprintf(stderr,"Operation not supported:\n");

     exit(1);

   }

    return 0; //not reached

 }
  
        
        
int add_op(int a,int b){

  return (a+b);

}

