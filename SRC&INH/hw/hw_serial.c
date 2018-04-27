#include "stm32l1xx.h"
#include "my_usb.h" 
#include <stdio.h>
#include <string.h>
#include "hw.h"

/*
       MG-TXD---PA9---USART1_TX---dma1_channel4
       MG-RXD---PA10--USART1_RX---dma1_channel5
*/
/*     TXDC-1---PB7   
C1<-------TX1---PA2---USART2_TX
C1------->RX1---PA3---USART2_RX

       TXDC-2---PB6    
C2<-------TX2---PC12---UART5_TX
C2------->RX2---PD2----UART5_RX*/
 
unsigned char USART_DMA_TxBuf[1]; 
unsigned char USART_DMA_RxBuf[1];  
/*usart1在portserial文件中实现
char USART1_DMA_Send_State; 
int  USART1_DMA_Rec_Cnt; 

void USART1_DMA_Send(unsigned short int lenth){ 
	int timeout=100000; 
	
	if(lenth <= 0){ 
	  USART1_DMA_Send_State = 0; 
		return ; 
	} 
	USART1_DMA_Send_State = 1; 
  DMA_SetCurrDataCounter(DMA1_Channel4,lenth); 
  DMA_Cmd(DMA1_Channel4,ENABLE); 
	while(USART1_DMA_Send_State && timeout-->0); 
	if(0 == timeout){
	  __NOP();
	}
} 
 
void USART1_Config(unsigned int baudrate){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	DMA1_Channel4_Config();
	DMA1_Channel5_Config();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
  USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	
	USART_ITConfig(USART1,USART_IT_TC,DISABLE);  
  USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;     
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
}*/
 
void DMA1_Channel4_Config(void){
	DMA_InitTypeDef  DMA_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);  
	
  DMA_DeInit(DMA1_Channel4); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)(&USART1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)USART_DMA_TxBuf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 99;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel4, DISABLE);
}

void DMA1_Channel5_Config(void){
	DMA_InitTypeDef  DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)(&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)USART_DMA_RxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 99;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel5, ENABLE);
}

//USART2 for C1 SDI12 receive
void USART2_SDI12_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
	GPIO_PinAFConfig(C1_RX_PORT,C1_RX_PIN_SOURCE,GPIO_AF_USART2);  
	GPIO_InitStructure.GPIO_Pin = C1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
  GPIO_Init(C1_RX_PORT, &GPIO_InitStructure);
	
  USART_InitStructure.USART_BaudRate = 1200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStructure);  
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  
	USART_Cmd(USART2,DISABLE);  
}
//UART5 for C2 SDI12 receive
void UART5_SDI12_Config(void){
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); 
	GPIO_PinAFConfig(C2_RX_PORT,C2_RX_PIN_SOURCE,GPIO_AF_UART5);  
	GPIO_InitStructure.GPIO_Pin = C2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
  GPIO_Init(C2_RX_PORT, &GPIO_InitStructure);
 
  USART_InitStructure.USART_BaudRate = 1200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_Init(UART5, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART5,DISABLE);
}

 
//USART2 for C1 RS232  receive
void USART2_RX_Config(USART_InitTypeDef sC_RS232_Init){
	GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_PinAFConfig(C1_RX_PORT,C1_RX_PIN_SOURCE,GPIO_AF_USART2);
	GPIO_InitStructure.GPIO_Pin = C1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
  GPIO_Init(C1_RX_PORT, &GPIO_InitStructure);
	 
	USART_Init(USART2, &sC_RS232_Init);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
}
 
//UART5 for C2 RS232 receive
void UART5_RX_Config(USART_InitTypeDef sC_RS232_Init){
  GPIO_InitTypeDef GPIO_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); 
	GPIO_PinAFConfig(C2_RX_PORT,C2_RX_PIN_SOURCE,GPIO_AF_UART5);
	GPIO_InitStructure.GPIO_Pin = C2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
  GPIO_Init(C2_RX_PORT,&GPIO_InitStructure);
	 
	USART_Init(UART5, &sC_RS232_Init);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART5,ENABLE);
} 
