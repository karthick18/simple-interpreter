#include<stdio.h>
#include<string.h>
#include<stdlib.h>

main(){
unsigned long *ptr;
char *s = "738";
ptr  = (unsigned long*)s;
printf("%#x\n",*ptr);
 printf("%#x\n",s);
 printf("%#x\n",s+1);
exit(0);
}
