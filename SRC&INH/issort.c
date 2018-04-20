#include "stdlib.h"
#include "string.h"
#include "issort.h"

int compare(const void *int1,const void *int2){
  if(*(const int *)int1 > *(const int *)int2){
	  return 1;
	}
	else if(*(const int *)int1 < *(const int *)int2){
	  return -1;
	}
	else{
	 return 0;
	}
}

int issort(void *data,int size,int esize,int (*compare)(const void *key1,const void *key2)){
  char *a=data;
	void *key;
	int i,j;
	
	if((key = (char *)malloc(esize)) == NULL){
	  return -1;
	}
	for(j=1;j<size;j++){
	  memcpy(key,&a[j*esize],esize);
		i = j-1;
		while(i>=0 && compare(&a[i*esize],key)>0){
		  memcpy(&a[(i+1)*esize],&a[i*esize],esize);
			i--;
		}
		memcpy(&a[(i+1)*esize],key,esize);
	}
	free(key);
	
	return 0;
}