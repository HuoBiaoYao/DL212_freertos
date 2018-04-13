#include "ctype.h" 
#include "string.h" 
#include "math.h"
#include "arithmetic_expression.h"
#include "DL212.h"
 
/*
&:表示取PUBLIC地址 */
char *Test_DL212_Exp="&3=-(&9-&2*(1024-(-(3^(-(1+&1))*2))))";//1118.25708
unsigned int DL212_Exp_Calculate(double *var,char *exp){ 
  int i=0,j=0,k=0,vn[10]={0},v=0; 
	double da1[50],result[50],*d[2],data,*p=&data,minus_1=-1; 
	char *op,mul='*'; 
	static Stack *s1,*s2; 
	
	if('&' != *(exp+i++)){ 
	  while(1); 
	} 
	vn[v] = atoi(exp+i)-1;//编号从1开始,要减一  
	if(vn[v]>PUBLIC_NUM_MAX+DIM_NUM_MAX || vn[v]<0){ 
	  while(1); 
	} 
  while('=' != *(exp+i)){ 
	  i++; 
	} 
	i++; 
	s1 = (Stack*)malloc(sizeof(Stack));//操作符
  s2 = (Stack*)malloc(sizeof(Stack));//操作数 
	stack_init(s1, NULL_P); 
	stack_init(s2, NULL_P); 
	//ToReversePolishNotation
	if(*(exp+i) == '-'){ 
		i++; 
	  if(isdigit(*(exp+i))){ 
		  da1[j] = (-1)*atof(exp+i); 
			while(isdigit(*(exp+i)) || *(exp+i)=='.'){ 
			  i++; 
			} 
			stack_push(s2,&da1[j++]); 
		}
		else if(*(exp+i) == '('){
		  da1[j] = -1;  
			stack_push(s2,&da1[j++]); 
			stack_push(s1,&mul); 
		}
		else{
		  while(1);
		}
	}
  while('\0'!=*(exp+i) && ','!=*(exp+i)){ 
		if('&' == *(exp+i)){ 
			i++;
		  vn[v] = atoi(exp+i)-1; 
			if(vn[v]>PUBLIC_NUM_MAX+DIM_NUM_MAX || vn[v]<0){
			  while(1);
			}
			da1[j] = *((float*)var+vn[v++]);
			stack_push(s2,&da1[j++]); 
      while(isdigit(*(exp+i))){ 
	      i++; 
	    } 
		} 
		else if(isdigit(*(exp+i))){ 
		  da1[j] = atof(exp+i);  
			while(isdigit(*(exp+i)) || *(exp+i)=='.'){ 
			  i++; 
			} 
			stack_push(s2,&da1[j++]);  
		} 
		else{ 
	    switch(*(exp+i)){ 
        case '(': 
          stack_push(s1,exp+i);
          i++; 				
			  break;
		    case ')':
				  while(*(char*)(s1->head->data) != '('){ 
						stack_pop(s1,(void**)&op); 
						stack_push(s2,op);
				  } 
					stack_pop(s1,(void**)&op);
          i++; 					
				break; 
				default: 
					if(*(exp+i)=='-' && *(exp+i-1) == '('){
            i++;
            if(isdigit(*(exp+i))){ 
							da1[j] = (-1)*atof(exp+i);  
							while(isdigit(*(exp+i)) || *(exp+i)=='.'){ 
								i++; 
							} 
							stack_push(s2,&da1[j++]);  
							if(*(exp+i) == ')'){
								stack_pop(s1,(void**)&op); 
								i++; 
							} 
						}
						
						else if(*(exp+i) == '('){
		          da1[j] = -1;  
			        stack_push(s2,&da1[j++]); 
			        stack_push(s1,&mul); 
		        }
						
						else{
							return 0x7FC00000;
						}
					}
					else{
					  if(s1->head==NULL_P || *(char*)(s1->head->data)=='('){
					    stack_push(s1,exp+i); 
					  }
	          else{ 
				      pri_cmp(s1,s2,exp+i); 
					  } 
						i++; 
					} 
				break; 
			}  
		}	
	}
	while(s1->head != NULL_P){ 
		stack_pop(s1,(void**)&op); 
	  stack_push(s2,op); 
	}
  ReverseStack(s1,s2);
	//calculate 
	j = 0;
	while(stack_size(s1) != 0){
	  stack_pop(s1,(void**)&p);
		if((unsigned int)p>=0x8000000 && (unsigned int)p<=0x8060000){ 
		  if(*(char*)p == '+'){/////bug
		    stack_pop(s2,(void**)&d[0]);
		  	stack_pop(s2,(void**)&d[1]); 
		  	result[j] = *d[1]+*d[0];
	  		stack_push(s2,&result[j]);
	  		j++;
	  	}
  		else if(*(char*)p == '-'){
	  	  stack_pop(s2,(void**)&d[0]);
	  		stack_pop(s2,(void**)&d[1]); 
	  		result[j] = *d[1]-*d[0];
		  	stack_push(s2,&result[j]);
		  	j++;
	  	}
	  	else if(*(char*)p == '*'){
	  	  stack_pop(s2,(void**)&d[0]);
	  		stack_pop(s2,(void**)&d[1]); 
		  	result[j] = *d[1]*(*d[0]);
		  	stack_push(s2,&result[j]);
		  	j++;
		  }
		  else if(*(char*)p == '/'){
		    stack_pop(s2,(void**)&d[0]);
		  	stack_pop(s2,(void**)&d[1]); 
		  	result[j] = *d[1]/(*d[0]);
		  	stack_push(s2,&result[j]);
		  	j++;
		  }
	  	else if(*(char*)p == '^'){
		    stack_pop(s2,(void**)&d[0]);
	  		stack_pop(s2,(void**)&d[1]); 
	  		result[j] = pow(*d[1],*d[0]);
	  		stack_push(s2,&result[j]);
	  		j++;
	  	}  
			else{
			  while(1);
			}
		}
    else if((unsigned int)p>=0x20000000 && (unsigned int)p<=0x2000C000){ 
		  stack_push(s2,p);
	  }
	}
	free(s1);
	free(s2);
  if(j > 0){
		*((float*)var+vn[0]) = (float)result[j-1];
	  return i; 
	}
	else{
	  return 0x7FC0000;
	}
} 
  
//char *Test_Exp="(1+2)*((110-4/0.4)-1)+111";//408 
//char *Test_Exp="1+((2+3)*4)-5";//16 
//char *Test_Exp="4*((2+3)/2)-5.1";//4.9 
//char *Test_Exp="((3+2)*(1+4+(1+1.11*(2.0+0.9))))/(2*3+3)";//5.121666 
//char *Test_Exp="(((10000000+99.20)/3+0.1*(9-3.0*(2-1.0))))";//3333367 
//char *Test_Exp="((1+2)/2)^(8-(1-0.4/2)^2*5)";//7.002256952814 
//char *Test_Exp="-2.5+(-0.3)*(-2+(-3)^3*2^(-2))*1.5";//1.4375 
//char *Test_Exp="-(-(-(2^2*3+(-2/(1+2)))))";//-11.33333333333 
//char *Test_Exp="2*(-(2^(-2)+(-(-(3*2^3*((-2^(1+1)+9)))))))";//-624.5 
//char *Test_Exp="-(9-2*(1024-(-(3^(-(1+1))*2))))";//2039.4444 
char *Test_Exp="-(-9^2-2*(1024-(-(3^(-(1+1))*2))))";//1967.4444 
#if 1 
double Exp_Calculate(char *exp){
	static int i=0,j=0,k=0,len=0; 
	double da1[50],result[50],*d[2],data,*p=&data,minus_1=-1;
	char *op,mul='*';
	static Stack *s1,*s2; 
	
	s1 = (Stack*)malloc(sizeof(Stack));//操作符
  s2 = (Stack*)malloc(sizeof(Stack));//操作数 
	stack_init(s1, NULL_P); 
	stack_init(s2, NULL_P); 
	//ToReversePolishNotation
	if(*(exp+i) == '-'){
		i++;
	  if(isdigit(*(exp+i))){ 
		  da1[j] = (-1)*atof(exp+i); 
			len = 0; 
			while(isdigit(*(exp+i+len)) || *(exp+i+len)=='.'){ 
			  len++; 
			} 
			stack_push(s2,&da1[j++]); 
			i += len; 
		}
		else if(*(exp+i) == '('){
		  da1[j] = -1;  
			stack_push(s2,&da1[j++]); 
			stack_push(s1,&mul); 
		}
		else{
		  return 0x7FC00000;
		}
	}
  while(*(exp+i) != '\0'){ 
		if(isdigit(*(exp+i))){ 
		  da1[j] = atof(exp+i); 
			len = 0; 
			while(isdigit(*(exp+i+len)) || *(exp+i+len)=='.'){ 
			  len++; 
			} 
			stack_push(s2,&da1[j++]); 
			i += len; 
		} 
		else{ 
	    switch(*(exp+i)){ 
        case '(': 
          stack_push(s1,exp+i);
          i++; 				
			  break;
		    case ')':
				  while(*(char*)(s1->head->data) != '('){ 
						stack_pop(s1,(void**)&op); 
						stack_push(s2,op);
				  } 
					stack_pop(s1,(void**)&op);
          i++; 					
				break; 
				default: 
					if(*(exp+i)=='-' && *(exp+i-1) == '('){
            i++;
            if(isdigit(*(exp+i))){ 
							da1[j] = (-1)*atof(exp+i); 
							len = 0; 
							while(isdigit(*(exp+i+len)) || *(exp+i+len)=='.'){ 
								len++; 
							} 
							stack_push(s2,&da1[j++]); 
							i += len; 
							if(*(exp+i) == ')'){
								stack_pop(s1,(void**)&op); 
								i++; 
							} 
						}
						
						else if(*(exp+i) == '('){
		          da1[j] = -1;  
			        stack_push(s2,&da1[j++]); 
			        stack_push(s1,&mul); 
		        }
						
						else{
							return 0x7FC00000;
						}
					}
					else{
					  if(s1->head==NULL_P || *(char*)(s1->head->data)=='('){
					    stack_push(s1,exp+i); 
					  }
	          else{ 
				      pri_cmp(s1,s2,exp+i); 
					  } 
						i++; 
					} 
				break; 
			}  
		}	
	}
	while(s1->head != NULL_P){ 
		stack_pop(s1,(void**)&op); 
	  stack_push(s2,op); 
	}
  ReverseStack(s1,s2);
	//calculate 
	i = 0;
	while(stack_size(s1) != 0){
	  stack_pop(s1,(void**)&p);
		if((unsigned int)p>=0x8000000 && (unsigned int)p<=0x8060000){ 
	  	if(*(char*)p == '+'){/////bug
		    stack_pop(s2,(void**)&d[0]);
			  stack_pop(s2,(void**)&d[1]); 
			  result[i] = *d[1]+*d[0];
		  	stack_push(s2,&result[i]);
		  	i++;
		  }
	  	else if(*(char*)p == '-'){
		    stack_pop(s2,(void**)&d[0]);
		  	stack_pop(s2,(void**)&d[1]); 
		  	result[i] = *d[1]-*d[0];
		  	stack_push(s2,&result[i]);
		  	i++;
		  }
		  else if(*(char*)p == '*'){
		    stack_pop(s2,(void**)&d[0]);
		  	stack_pop(s2,(void**)&d[1]); 
		  	result[i] = *d[1]*(*d[0]);
		  	stack_push(s2,&result[i]);
		  	i++;
	  	}
	  	else if(*(char*)p == '/'){
		    stack_pop(s2,(void**)&d[0]);
		  	stack_pop(s2,(void**)&d[1]); 
		  	result[i] = *d[1]/(*d[0]);
		   	stack_push(s2,&result[i]);
		  	i++;
	  	}
		  else if(*(char*)p == '^'){
		    stack_pop(s2,(void**)&d[0]);
			  stack_pop(s2,(void**)&d[1]); 
		  	result[i] = pow(*d[1],*d[0]);
		  	stack_push(s2,&result[i]);
		  	i++;
		  }  
			else{
			  while(1);
			}
	  }
		else if((unsigned int)p>=0x20000000 && (unsigned int)p<=0x2000C000){ 
		  stack_push(s2,p);
		}
	}
	free(s1);
	free(s2);
  if(i > 0){
	  return result[i-1];
	}
	else{
	  return 0x7FC0000;
	}
}
#endif

#if 0
/*可以处理负数，不能处理负表达式，负表达式需要上位机处理*/
double Exp_Calculate(char *exp){
	static int i=0,j=0,k=0,len=0; 
	double da1[50],result[50],*d[2],data,*p=&data,minus_1=-1;
	char *op,mul='*';
	static Stack *s1,*s2; 
	
	s1 = (Stack*)malloc(sizeof(Stack));//操作符
  s2 = (Stack*)malloc(sizeof(Stack));//操作数 
	stack_init(s1, NULL_P); 
	stack_init(s2, NULL_P); 
	//ToReversePolishNotation
	if(*(exp+i) == '-'){
		i++;
	  if(isdigit(*(exp+i))){ 
		  da1[j] = (-1)*atof(exp+i); 
			len = 0; 
			while(isdigit(*(exp+i+len)) || *(exp+i+len)=='.'){ 
			  len++; 
			} 
			stack_push(s2,&da1[j++]); 
			i += len; 
		}
		else{
		  return 0x7FC00000;
		}
	}
  while(*(exp+i) != '\0'){ 
		if(isdigit(*(exp+i))){ 
		  da1[j] = atof(exp+i); 
			len = 0; 
			while(isdigit(*(exp+i+len)) || *(exp+i+len)=='.'){ 
			  len++; 
			} 
			stack_push(s2,&da1[j++]); 
			i += len; 
		} 
		else{ 
	    switch(*(exp+i)){ 
        case '(': 
          stack_push(s1,exp+i);
          i++; 				
			  break;
		    case ')':
				  while(*(char*)(s1->head->data) != '('){ 
						stack_pop(s1,(void**)&op); 
						stack_push(s2,op);
				  } 
					stack_pop(s1,(void**)&op);
          i++; 					
				break; 
				default: 
					if(*(exp+i)=='-' && *(exp+i-1) == '('){
            i++;
            if(isdigit(*(exp+i))){ 
							da1[j] = (-1)*atof(exp+i); 
							len = 0; 
							while(isdigit(*(exp+i+len)) || *(exp+i+len)=='.'){ 
								len++; 
							} 
							stack_push(s2,&da1[j++]); 
							i += len; 
							if(*(exp+i) == ')'){
								stack_pop(s1,(void**)&op); 
								i++; 
							} 
						}
						else{
							return 0x7FC00000;
						}
					}
					else{
					  if(s1->head==NULL_P || *(char*)(s1->head->data)=='('){
					    stack_push(s1,exp+i); 
					  }
	          else{ 
				      pri_cmp(s1,s2,exp+i); 
					  } 
						i++; 
					} 
				break; 
			}  
		}	
	}
	while(s1->head != NULL_P){ 
		stack_pop(s1,(void**)&op); 
	  stack_push(s2,op); 
	}
  ReverseStack(s1,s2);
	//calculate 
	i = 0;
	while(stack_size(s1) != 0){
	  stack_pop(s1,(void**)&p);
		if(*(char*)p == '+'){
		  stack_pop(s2,(void**)&d[0]);
			stack_pop(s2,(void**)&d[1]); 
			result[i] = *d[1]+*d[0];
			stack_push(s2,&result[i]);
			i++;
		}
		else if(*(char*)p == '-'){
		  stack_pop(s2,(void**)&d[0]);
			stack_pop(s2,(void**)&d[1]); 
			result[i] = *d[1]-*d[0];
			stack_push(s2,&result[i]);
			i++;
		}
		else if(*(char*)p == '*'){
		  stack_pop(s2,(void**)&d[0]);
			stack_pop(s2,(void**)&d[1]); 
			result[i] = *d[1]*(*d[0]);
			stack_push(s2,&result[i]);
			i++;
		}
		else if(*(char*)p == '/'){
		  stack_pop(s2,(void**)&d[0]);
			stack_pop(s2,(void**)&d[1]); 
			result[i] = *d[1]/(*d[0]);
			stack_push(s2,&result[i]);
			i++;
		}
		else if(*(char*)p == '^'){
		  stack_pop(s2,(void**)&d[0]);
			stack_pop(s2,(void**)&d[1]); 
			result[i] = pow(*d[1],*d[0]);
			stack_push(s2,&result[i]);
			i++;
		}  
		else{
		  stack_push(s2,p);
		} 
	}
	free(s1);
	free(s2);
  if(i > 0){
	  return result[i-1];
	}
	else{
	  return 0x7FC0000;
	}
}
#endif


char pri_cmp(Stack *s1,Stack *s2,char *op){
	char pri1=0,pri2=0;
	char *s1_op;
	
	if(*(char*)(s1->head->data) == '('){
		stack_push(s1,op);
	  return 0;
	}
	if(s1->size == 0){
	  stack_push(s1,op);
		return 0;
	}
	if(*(char*)s1->head->data == '^'){
	  pri1 = 3;
	}
	else if(*(char*)s1->head->data=='*' || *(char*)s1->head->data=='/'){
	  pri1 = 2;
	}
	else{
	  pri1 = 1;   
	}
	if(*op == '^'){
	  pri2 = 3;
	}
	else if(*op=='*' || *op=='/'){
	  pri2 = 2;
	}
	else{
	  pri2 = 1;   
	}
	if(pri2 > pri1){
	  stack_push(s1,op);
	}
	else{ 
		stack_pop(s1,(void**)&s1_op);
		stack_push(s2,s1_op);
	  pri_cmp(s1,s2,op);
	}
   
	return  0;
}

void ReverseStack(Stack *new_s,Stack *old_s){ 
	double data=0,*p=&data;
	
  if(0 == old_s->size){
	  return;
	} 
  stack_push(new_s,old_s->head->data);
	stack_pop(old_s,(void**)&p);
	ReverseStack(new_s,old_s);
}
 
 