#include<stdio.h>
#include<hash.h>

unsigned int make_hash(unsigned char *key){

  unsigned int hashval=0;

  for(;*key;key ++)

    hashval = hashval + (*key) * 31;
  
  return (hashval % HASH_SIZE);

}
