#ifndef __ARITHMETIC_EXPRESSION_H
#define __ARITHMETIC_EXPRESSION_H
#include "stack.h" 

extern char *Test_DL212_Exp;
extern char *Test_Exp;

extern unsigned int DL212_Exp_Calculate(double *var,char *exp);
extern double Exp_Calculate(char *exp);
extern char pri_cmp(Stack *s1,Stack *s2,char *op);
extern void ReverseStack(Stack *new_s,Stack *old_s);
#endif
