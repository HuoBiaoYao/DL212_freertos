#include "hal_c_port.h"
#include "stm32l1xx_gpio.h"


_C_PORT_STATE eC_PORT_STATE[2];
  
struct _C_OUT_Func sC_OUT_Func={
	C_OUT_Init,
  C_OUT_State
};
struct _C_Pulse_Func sC_Pulse_Func={
  C_Pulse_Init
};

struct _C_OUT_Func   *psC_OUT_Func=&sC_OUT_Func;
struct _C_Pulse_Func *psC_Pulse_Func=&sC_Pulse_Func;

void C_OUT_Init(unsigned char port){ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	eC_PORT_STATE[port] = C_OUT_IN_USE;
  switch(port){
	  case 0:
			GPIO_InitStructure.GPIO_Pin = C1_CTRL_PIN;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
			GPIO_Init(C1_CTRL_PORT, &GPIO_InitStructure);
		  
		  GPIO_InitStructure.GPIO_Pin = C1_TX_PIN;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		  GPIO_Init(C1_TX_PORT,&GPIO_InitStructure); 	 
		break;
		case 1:
			GPIO_InitStructure.GPIO_Pin = C2_CTRL_PIN;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
			GPIO_Init(C2_CTRL_PORT, &GPIO_InitStructure);
		
		  GPIO_InitStructure.GPIO_Pin = C2_TX_PIN;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		  GPIO_Init(C2_TX_PORT,&GPIO_InitStructure); 	 	 
		break;
		default: 
		break;
	}
}

void C_OUT_State(unsigned char port,unsigned char state){
  switch(port){
	  case 0:
			SDI12_C1_SEND_ENABLE();
		  if(state){
			  SDI12_C1_HIGH();
			}
			else{
			  SDI12_C1_LOW();
			}
		break;	
		case 1:
			SDI12_C2_SEND_ENABLE();
			if(state){
			  SDI12_C2_HIGH();
			}
			else{
			  SDI12_C2_LOW();
			}
		break;
		default:
		break;
	}
}


void C_Pulse_Init(unsigned char port){
  GPIO_InitTypeDef GPIO_InitStructure;
	
	eC_PORT_STATE[port] = C_PULSE_IN_USE;
  switch(port){
	  case 0:
			GPIO_InitStructure.GPIO_Pin = C1_CTRL_PIN;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
			GPIO_Init(C1_CTRL_PORT, &GPIO_InitStructure);
		  
		  SDI12_C1_SEND_DISABLE();
		  
		  GPIO_InitStructure.GPIO_Pin = C1_TX_PIN;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		  GPIO_Init(C1_TX_PORT,&GPIO_InitStructure); 	 
		break;
		case 1:
			GPIO_InitStructure.GPIO_Pin = C2_CTRL_PIN;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
			GPIO_Init(C2_CTRL_PORT, &GPIO_InitStructure);
		  
		  SDI12_C2_SEND_DISABLE();
		  
		  GPIO_InitStructure.GPIO_Pin = C2_TX_PIN;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		  GPIO_Init(C2_TX_PORT,&GPIO_InitStructure); 	 	 
		break;
		default: 
		break;
	}
}
