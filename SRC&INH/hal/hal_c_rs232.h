#ifndef __HAL_C_RS232_H
#define __HAL_C_RS232_H
#include "stm32l1xx_usart.h"


#define C_RS232_RX_SIZE   200
#define C_RS232_TX_SIZE   200

struct _C_RS232_PARA{
  unsigned char tx_buf[C_RS232_TX_SIZE];
	unsigned char rx_buf[C_RS232_RX_SIZE];
	unsigned int lenth;
	unsigned int rx_ptr;
	unsigned int tx_ptr;
};

struct _C_RS232_FUNC{
  void (*init)(unsigned char port,unsigned char mode);
	void (*read)(unsigned char port,unsigned char *dst,unsigned char *src);
	void (*send)(unsigned char port,unsigned char *buf,unsigned int len);
	void (*sendbyte)(unsigned char port,USART_InitTypeDef,unsigned char *byte);
	void (*send_enable)(unsigned char port);
	void (*send_disable)(unsigned char port); 
	void (*bus_state)(unsigned char port,unsigned char state);
	void (*delay)(unsigned int baudrate);
};
  
void C_RS232_Init(unsigned char port,unsigned char mode);
void C_RS232_Read(unsigned char port,unsigned char *dst,unsigned char *src);
void C_RS232_Send(unsigned char port,unsigned char *buf,unsigned int len);
void C_RS232_Sendbyte(unsigned char port,USART_InitTypeDef,unsigned char *byte);
void C_RS232_Send_enable(unsigned char port);
void C_RS232_Send_disable(unsigned char port);
void C_RS232_Bus_State(unsigned char port,unsigned char state);
void C_RS232_Delay(unsigned int baudrate);
 
extern struct _C_RS232_FUNC sC_RS232_Func;
extern struct _C_RS232_FUNC *psC_RS232_Func;

extern struct _C_RS232_PARA sC_RS232_Para[2];
extern USART_InitTypeDef sC_RS232_Init[2];
#endif
