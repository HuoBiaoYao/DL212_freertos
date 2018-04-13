#ifndef __HAL_PWR_H
#define __HAL_PWR_H
#include "main.h"

extern struct _Power_Func sPower_Func;
extern struct _Power_Func *psPower_Func;

struct _Power_Func{
  void (*init)(void);
	void (*sw)(unsigned char powertype,unsigned char state);
};


#endif
