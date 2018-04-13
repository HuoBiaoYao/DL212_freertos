#ifndef __HAL_SW12_H
#define __HAL_SW12_H
#include "main.h"


extern struct _SW12_Func sSW12_Func;
extern struct _SW12_Func *psSW12_Func;

struct _SW12_Func{
  void (*init)(void);
	void (*sw)(unsigned char port,unsigned char state);
};

#endif
