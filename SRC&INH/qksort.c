#include "stdlib.h"
#include "string.h"
#include "qksort.h"

int compare_int(const void *int1,const void *int2){
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

int partition(void *data,int esize,int i,int k,int (*compare)(const void *key1,const void *key2)){
  char *a=data;
	void *pval,*temp;
	int r[3];
	
	if((pval = malloc(esize)) == NULL){
	  return -1;
	}
	if((temp = malloc(esize)) == NULL){
	  free(pval);
		return -1;
	}
	r[0] =(rand()%(k-i+1))+i;
	r[1] =(rand()%(k-i+1))+i;
	r[2] =(rand()%(k-i+1))+i;
	//issort(r,3,sizeof(int),compare_int);
	memcpy(pval,&a[r[1]*esize],esize);
	i--;
	k++;
	while(1);
		  //Î´Íê´ıĞø

	
	return 0;
}