#include<stdio.h>

struct sample {

  int a;
};

struct sample *traverse;

int test_operate(){
  traverse = (struct sample*) malloc(sizeof(struct sample) );

  traverse->a = 4;

  printf("Sample:%d\n",traverse->a);

  free((void*)traverse);

}
