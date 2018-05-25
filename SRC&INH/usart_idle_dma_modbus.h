#ifndef __USART_IDLE_DMA_MODBUS
#define __USART_IDLE_DMA_MODBUS
/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"
/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbfunc.h"

#include "mbport.h"
#include "mbrtu.h"

#define MB_UART_DEV                ( USART1 )
#define MB_UART_CLOCK              ( RCC_APB2Periph_USART1 ) 
#define MB_UART_PORT_CLOCK         ( RCC_AHBPeriph_GPIOA )
#define MB_UART_TX_PORT            ( GPIOA )
#define MB_UART_TX_PIN             ( GPIO_Pin_9 )
#define MB_UART_RX_PORT            ( GPIOA )
#define MB_UART_RX_PIN             ( GPIO_Pin_10 )
#define MB_UART_IRQ_CH             ( USART1_IRQn )
#define MB_Slave_USART_IRQHandler  ( USART1_IRQHandler )
#define MB_Slave_DMA_TX_IRQHandler ( DMA1_Channel4_IRQHandler )
 
struct MB_FUNCTION{
  void (*init)(unsigned int baudrate); 
	void (*poll)(void);
	void (*send)(UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength );
};

extern struct MB_FUNCTION sMBSlave;

void MB_Slave_Task(void); 
void MB_Slave_Poll(void);
void MB_Slave_Send(UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength );
#endif
