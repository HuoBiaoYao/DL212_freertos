#ifndef __HW_SERIAL_H
#define __HW_SERIAL_H  
#include "stm32l1xx_usart.h"

extern char USART1_DMA_Send_State;
extern unsigned int USART1_DMA_Rec_Cnt;
extern unsigned char USART_DMA_TxBuf[];
extern unsigned char USART_DMA_RxBuf[];
 
void USART1_DMA_Send(unsigned short int lenth);
void USART1_Config(unsigned int baudrate);
void DMA1_Channel4_Config(void);
void DMA1_Channel5_Config(void);

void USART2_SDI12_Config(void);
void UART5_SDI12_Config(void); 

void USART2_RX_Config(USART_InitTypeDef sC_RS232_Init);
void UART5_RX_Config(USART_InitTypeDef sC_RS232_Init);
#endif
