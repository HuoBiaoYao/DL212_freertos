#ifndef __STACK_H
#define __STACK_H

#include "stdlib.h"
#include "mylist.h"

typedef List Stack;

#define stack_init list_init
#define stack_destroy list_destroy

#define stack_peek(stack) ((stack)->head==NULL_P?NULL_P:(stack)->head->data)
#define stack_size list_size

int stack_push(Stack *stack,const void *data);
int stack_pop(Stack *stack,void **data);
 

#endif
