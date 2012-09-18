#include<stdio.h>
#include<check.h>

main(){

 char src[100];
 char *what = "karthick";
 char *by = "radha";

 char *ptr =src;
 strcpy(ptr ,"karthick is a good boy and karthick loves radha like crazy");
 
 str_replace(ptr,what,by);

 printf("Replaced string:%s:\n",ptr);

 exit(0);

 }

 
