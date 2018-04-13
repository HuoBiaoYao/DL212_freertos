#include "hal.h"
#include "hw.h"
#include "delay.h"
#include "stm32l1xx_gpio.h"

void C_RS232_Sendbyte_IRQ(unsigned char port,USART_InitTypeDef sC_RS232_Init,unsigned char *byte);
/*串口可配置参数包括：波特率、奇偶校验、*/
USART_InitTypeDef sC_RS232_Init[2];
struct _C_RS232_PARA sC_RS232_Para[2];

struct _C_RS232_FUNC sC_RS232_Func={
  C_RS232_Init,
  C_RS232_Read,
  C_RS232_Send,
  C_RS232_Sendbyte, 
  C_RS232_Send_enable,
  C_RS232_Send_disable,
	C_RS232_Bus_State,
	C_RS232_Delay
};
struct _C_RS232_FUNC *psC_RS232_Func=&sC_RS232_Func;

void C_RS232_Init(unsigned char port,unsigned char mode){
	GPIO_InitTypeDef GPIO_InitStructure;
  
  sC_RS232_Init[1].USART_BaudRate = sC_RS232_Init[0].USART_BaudRate  = 115200;
	sC_RS232_Init[1].USART_WordLength = sC_RS232_Init[0].USART_WordLength = USART_WordLength_9b;
	sC_RS232_Init[1].USART_StopBits = sC_RS232_Init[0].USART_StopBits = USART_StopBits_1;
	sC_RS232_Init[1].USART_Parity =sC_RS232_Init[0].USART_Parity = USART_Parity_No;
	sC_RS232_Init[1].USART_HardwareFlowControl = sC_RS232_Init[0].USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	sC_RS232_Init[1].USART_Mode = sC_RS232_Init[0].USART_Mode = USART_Mode_Rx;
  
	if(0 == mode){
	  eC_PORT_STATE[port] = C_RS232_RX_IN_USE;
	}
	else{
	  eC_PORT_STATE[port] = C_RS232_TX_IN_USE;
	}
	switch(port){
    case 0:
      USART2_RX_Config(sC_RS232_Init[0]);
		  GPIO_InitStructure.GPIO_Pin = C1_CTRL_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_Init(C1_CTRL_PORT, &GPIO_InitStructure);
		  psC_RS232_Func->send_disable(0);
		
		  GPIO_InitStructure.GPIO_Pin = C1_TX_PIN;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		  GPIO_Init(C1_TX_PORT,&GPIO_InitStructure); 
    break;
    case 1:
      UART5_RX_Config(sC_RS232_Init[1]);
		  GPIO_InitStructure.GPIO_Pin = C2_CTRL_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_Init(C2_CTRL_PORT, &GPIO_InitStructure);
		  psC_RS232_Func->send_disable(1);
		
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

void C_RS232_Read(unsigned char port,unsigned char *dst,unsigned char *src){
	if(C_RS232_RX_IN_USE == eC_PORT_STATE[port]){
	  *(dst+sC_RS232_Para[port].rx_ptr++) = *src;
		if(sC_RS232_Para[port].rx_ptr >= C_RS232_RX_SIZE){
		  sC_RS232_Para[port].rx_ptr = 0;
		}
 	}
}

void C_RS232_Send(unsigned char port,unsigned char *buf,unsigned int len){
	if(C_RS232_TX_IN_USE != eC_PORT_STATE[port]){
	  return;
	}
	psC_RS232_Func->send_enable(port); 
	while(len--){
	  sC_RS232_Func.sendbyte(0,sC_RS232_Init[port],buf++);
	}
	psC_RS232_Func->send_disable(port);
}

void C_RS232_Sendbyte(unsigned char port,USART_InitTypeDef sC_RS232_Init,unsigned char *byte){
	unsigned char data,i,len,even=0,odd=0;
	
	if(USART_Parity_No == sC_RS232_Init.USART_Parity){
	  len = 8;
	}
	else{
	  len = 7;
	}
	data = *byte;
	psC_RS232_Func->bus_state(port,1);
	psC_RS232_Func->delay(sC_RS232_Init.USART_BaudRate);
	for(i=0;i<len;i++){
		if(data&0x01){
			psC_RS232_Func->bus_state(port,0);
			psC_RS232_Func->delay(sC_RS232_Init.USART_BaudRate);
		  even++;
		}
		else{
		  psC_RS232_Func->bus_state(port,1);
			psC_RS232_Func->delay(sC_RS232_Init.USART_BaudRate);
			odd++;
		}
		data >>= 1;
	}
	if(USART_Parity_Even == sC_RS232_Init.USART_Parity){
	  if(even&0x01){
	    psC_RS232_Func->bus_state(port,0); 
	  }
	  else{
      psC_RS232_Func->bus_state(port,1); 
	  }
		psC_RS232_Func->delay(sC_RS232_Init.USART_BaudRate);
	}
	else if(USART_Parity_Odd == sC_RS232_Init.USART_Parity){
	  if(odd&0x01){
	    psC_RS232_Func->bus_state(port,0); 
	  }
	  else{
      psC_RS232_Func->bus_state(port,1); 
	  }
		psC_RS232_Func->delay(sC_RS232_Init.USART_BaudRate);
	}
	psC_RS232_Func->bus_state(port,0);
	psC_RS232_Func->delay(sC_RS232_Init.USART_BaudRate);
} 

void C_RS232_Send_enable(unsigned char port){
  switch(port){
    case 0:
			if(eC_PORT_STATE[port] == C_RS232_TX_IN_USE){
        SDI12_C1_SEND_ENABLE();
			}
    break;
    case 1:
		  if(eC_PORT_STATE[port] == C_RS232_TX_IN_USE){
        SDI12_C2_SEND_ENABLE();
			}
    break;
    default:
    break;
  }
}

void C_RS232_Send_disable(unsigned char port){
  switch(port){
    case 0:
      SDI12_C1_SEND_DISABLE();
    break;
    case 1:
			SDI12_C2_SEND_DISABLE();
    break;
    default:
    break;
  }
}

void C_RS232_Bus_State(unsigned char port,unsigned char state){
  switch(port){
    case 0:
			if(state){
			  SDI12_C1_HIGH();
			}
			else{
			  SDI12_C1_LOW();
			}
    break;
    case 1:
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

void C_RS232_Delay(unsigned int baudrate){
	volatile unsigned char i=0;
	
  switch(baudrate){
	  case 300:
			delay_us(DElAY_Baud300);
		break;
		case 1200:
			delay_us(DElAY_Baud1200);
		break;
		case 2400:
			delay_us(DElAY_Baud2400);
		break;
		case 4800:
			delay_us(DElAY_Baud4800);
		break;
		case 9600:
			delay_us(DElAY_Baud9600);
		break;
		case 14400:
			delay_us(DElAY_Baud14400);
		break;
		case 19200:
			delay_us(DElAY_Baud19200);
		break;
		case 38400:
			delay_us(DElAY_Baud38400);
		break;
    case 115200:
		  while(i++<14);
		break; 
	}
}
 


