#ifndef __ISSORT_H
#define __ISSORT_H

int compare(const void *int1,const void *int2);
int issort(void *data,int size,int esize,int (*compare)(const void *key1,const void *key2));

#endif
