#ifndef __HAL_FRAM_H
#define __HAL_FRAM_H

extern struct _Fram_Func sFram_Func;
extern struct _Fram_Func *psFram_Func;

struct _Fram_Func{
  void (*init)(void);
	void (*erase)(unsigned int len);
	void (*write)(unsigned int addr,unsigned char *data,unsigned int len);
	void (*read) (unsigned int addr,unsigned char *data,unsigned int len);
	void (*test)(void);
};

void Fram_Test(void);
#endif
