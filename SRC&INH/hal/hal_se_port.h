#ifndef __HAL_SE_PORT_H
#define __HAL_SE_PORT_H

#include "hw_se_port.h"

struct _SE_FUNC{
	void (*init)(void); 
	void (*chan_open)(unsigned char chan);
	void (*chan_close)(void);
	float(*read)(unsigned char chan); 
	float(*bat_read)(void);
	float(*vref_read)(void);
	void (*out_init)(unsigned char chan);
	void (*out_ctrl)(unsigned char chan,unsigned char state);
};

extern struct _SE_FUNC sSE_FUNC;
extern struct _SE_FUNC *psSE_FUNC;

#endif
