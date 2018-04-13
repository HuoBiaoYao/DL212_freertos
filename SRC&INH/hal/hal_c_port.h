#ifndef  __HAL_C_PORT_H
#define  __HAL_C_PORT_H
#include "hal_sdi12.h"
  
 
struct _C_OUT_Func{
  void (*init)(unsigned char port);
	void (*out)(unsigned char port,unsigned char state);
}; 

struct _C_Pulse_Func{
  void (*init)(unsigned char port);
};
 
 
void C_OUT_Init(unsigned char port);
void C_OUT_State(unsigned char port,unsigned char state);
void C_Pulse_Init(unsigned char port);
 
extern struct _C_OUT_Func  sC_OUT_Func;
extern struct _C_OUT_Func *psC_OUT_Func;;
extern struct _Pulse_Func  sPulse_Func; 
extern struct _C_Pulse_Func *psC_Pulse_Func;
#endif
