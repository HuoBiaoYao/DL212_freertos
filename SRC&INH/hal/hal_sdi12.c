#include "string.h" 
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "hal.h"
#include "hw.h"  
#include "delay.h"
#include "my_usb.h"   
#include "stm32l1xx_gpio.h"
 
unsigned int SDI12_Data_Bytes[2];

__SDI12_BUS eSDI12_BUS[2];
struct _SDI12_PARA sSDI12_Para[2];
struct _SDI12_FUNC sSDI12_Func={
  SDI12_Init,
  SDI12_Transparent,
  SDI12_Read_FromISR,
  SDI12_Send,
	SDI12_SendByte,
	SDI12_Send_Enable,
	SDI12_Send_Disable,
	SDI12_High,
  SDI12_Low,
  SDI12_Start,
  SDI12_Stop
};
struct _SDI12_FUNC *psSDI12_Func=&sSDI12_Func;


void SDI12_Init(unsigned char port){
	GPIO_InitTypeDef GPIO_InitStructure;
 
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
			USART2_SDI12_Config();
		  eSDI12_BUS[port] = SDI12_IDLE;
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
			UART5_SDI12_Config(); 
		  eSDI12_BUS[port] = SDI12_IDLE;
		break;
		default: 
		break;
	}
} 

__SDI12_RSL SDI12_Read(unsigned char port,unsigned char *dst,unsigned char *src){
  if(SDI12_REICEIVE == eSDI12_BUS[port]){
	  *(dst+sSDI12_Para[port].rx_ptr++) = (*src&0x7F);
		if(sSDI12_Para[port].rx_ptr >= SDI12_RX_SIZE){
		  sSDI12_Para[port].rx_ptr = 0;
		}
	}
	
	return SDI12_OK;
}

__SDI12_RSL SDI12_Read_FromISR(unsigned char port,unsigned char *dst,unsigned char *src){ 
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 
	if(SDI12_REICEIVE == eSDI12_BUS[port]){
		*(dst+sSDI12_Para[port].rx_ptr++) = (*src&0x7F);
		if(1 == sSDI12_Para[port].rx_ptr){
		  if(BinarySemaphore_SDI12_FirstByte != NULL){
			  xSemaphoreGiveFromISR(BinarySemaphore_SDI12_FirstByte,&xHigherPriorityTaskWoken);
			}
		}
    if(0x0D==*(sSDI12_Para[port].rx_buf+sSDI12_Para[port].rx_ptr-2) && 0x0A==*(sSDI12_Para[port].rx_buf+sSDI12_Para[port].rx_ptr-1)){
      xSemaphoreGiveFromISR(BinarySemaphore_SDI12_CR_LF,&xHigherPriorityTaskWoken);
    }
		if(sSDI12_Para[port].rx_ptr >= SDI12_RX_SIZE){
		  sSDI12_Para[port].rx_ptr = 0;
		}
	}		
	
	return SDI12_OK;
}
			
__SDI12_RSL SDI12_Send(unsigned char port,unsigned char *string,unsigned int len){
	unsigned int i=0;
 
	eSDI12_BUS[port] = SDI12_SEND;
	psSDI12_Func->send_enable(port);
	switch(port){
	  case 0:
			USART_Cmd(USART2,DISABLE);
		break;
		case 1:
			USART_Cmd(UART5,DISABLE);
		break;
		default: 
		break;
	}
	psSDI12_Func->bus_high(port);
	delay_us(13000);
	psSDI12_Func->bus_low(port);
	switch(port){
	  case 0:
			USART_Cmd(USART2,ENABLE);
		break;
		case 1:
			USART_Cmd(UART5,ENABLE);
		break;
		default: 
		break;
	}
	delay_us(9000); 
	while(i < len){
		psSDI12_Func->sendbyte(port,string+i),i++;
	} 
	psSDI12_Func->send_disable(port); 
	eSDI12_BUS[port] = SDI12_BUSY;
	
	return SDI12_OK;
} 

__SDI12_RSL SDI12_SendByte(unsigned char port,unsigned char *byte){
	unsigned char i,even=0,buf;
 
	buf = *byte;
	psSDI12_Func->start(port);
	delay_us(DElAY_Baud1200);
	for(i=0;i<7;i++){
		if(buf&0x01){
			psSDI12_Func->bus_low(port);
			delay_us(DElAY_Baud1200);
		  even++;
		}
		else{
		  psSDI12_Func->bus_high(port);
			delay_us(DElAY_Baud1200);
		}
		buf >>= 1;
	}
	if(even&0x01){
	  psSDI12_Func->bus_low(port);
		delay_us(DElAY_Baud1200);
	}
	else{
	  psSDI12_Func->bus_high(port);
		delay_us(DElAY_Baud1200);
	} 
	psSDI12_Func->stop(port);
	delay_us(DElAY_Baud1200);
	
	return SDI12_OK;
}

void SDI12_Send_Enable(unsigned char port){
  switch(port){
	  case 0:
			 SDI12_C1_SEND_ENABLE();
		break;
		case 1:
			 SDI12_C2_SEND_ENABLE();
		break;
		default: 
		break;
	}
}

void SDI12_Send_Disable(unsigned char port){
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

void SDI12_High(unsigned char port){
	switch(port){
	  case 0:
			SDI12_C1_HIGH();
		break;	
		case 1:
			SDI12_C2_HIGH();
		break;
		default:
		break;
	}
}

void SDI12_Low(unsigned char port){
  switch(port){
	  case 0:
			SDI12_C1_LOW();
		break;	
		case 1:
			SDI12_C2_LOW();
		break;
		default:
		break;
	}
}

void SDI12_Start(unsigned char port){
  switch(port){
	  case 0:
			SDI12_C1_HIGH();
		break;	
		case 1:
			SDI12_C2_HIGH();
		break;
		default:
		break;
	}
}

void SDI12_Stop(unsigned char port){
  switch(port){
	  case 0:
			SDI12_C1_LOW();
		break;	
		case 1:
			SDI12_C2_LOW();
		break;
		default:
		break;
	}
}

__SDI12_RSL SDI12_Transparent(unsigned char port){
	if(0 == sUSB_Para.packet_rec){
		return 1; 
	}
	memcpy(sSDI12_Para[port].tx_buf ,sUSB_Para.rx_buf,sUSB_Para.rec_len);  
	sSDI12_Para[port].tx_ptr = sUSB_Para.rec_len;
  sUSB_Para.rec_len = sUSB_Para.rec_ptr = 0;
	if(sSDI12_Para[port].tx_buf[0]=='?' || isalnum(sSDI12_Para[port].tx_buf[0])){ 
		if('!' == sSDI12_Para[port].tx_buf[sSDI12_Para[port].tx_ptr-1]){ 
	    eSDI12_BUS[port] = SDI12_BUSY;
			sSDI12_Para[port].rx_buf[0] = sSDI12_Para[port].rx_ptr = 0;
			psSDI12_Func->send(port,sSDI12_Para[port].tx_buf,sSDI12_Para[port].tx_ptr);
	    delay_us(8000);
	    eSDI12_BUS[port] = SDI12_REICEIVE;
	    if(xSemaphoreTake(BinarySemaphore_SDI12_FirstByte,80) == pdTRUE){
				if(xSemaphoreTake(BinarySemaphore_SDI12_CR_LF,700) == pdTRUE){
			    USB_Send(sSDI12_Para[port].rx_buf,sSDI12_Para[port].rx_ptr); 
				}
			}  
		}
  } 
	eSDI12_BUS[port] = SDI12_IDLE;	
  sSDI12_Para[port].tx_buf[0] = sSDI12_Para[port].tx_ptr =  sSDI12_Para[port].rx_buf[0] = sSDI12_Para[port].rx_ptr = 0;	 
 
	return SDI12_OK;
}
 
char SDI12_Data_Ascii[2][400];
unsigned int SDI12_Data_Ascii_Cnt[2]={0,0};
__SDI12_RSL SDI12Recorder(char port,unsigned char *sdicmd){ 
	unsigned int retry,i=0,j=0,M_cmd_delay_ms=0;
	
	if(0 == *(sdicmd)){
	  return 1;
	}
	while(*(sdicmd+i) != 0){
	  if(*(sdicmd+i) == '!'){
			retry = 1;
		  while(retry--){
	  	  eSDI12_BUS[port] = SDI12_BUSY; 
		    psSDI12_Func->send(port,sdicmd+j,i-j+1);  
				delay_us(8000);
		    eSDI12_BUS[port] = SDI12_REICEIVE; 			
				
						if(xSemaphoreTake(BinarySemaphore_SDI12_FirstByte,80) == pdTRUE){
								if(xSemaphoreTake(BinarySemaphore_SDI12_CR_LF,700) == pdTRUE){
									if(*sSDI12_Para[port].rx_buf == *(sdicmd+j)){
										if(*(sdicmd+j+1) == 'M'){
											sSDI12_Para[port].rx_ptr = 0;
											M_cmd_delay_ms = ((*(sSDI12_Para[port].rx_buf+1)-0x30)*100000
											                 +(*(sSDI12_Para[port].rx_buf+2)-0x30)*10000
											                 +(*(sSDI12_Para[port].rx_buf+3)-0x30))*1000; 
											if(xSemaphoreTake(BinarySemaphore_SDI12_CR_LF,M_cmd_delay_ms) == pdTRUE){
												sSDI12_Para[port].rx_ptr = 0;
												break;
											}
										} 
										else if(*(sdicmd+j+1) == 'A'){//½ûÖ¹ÐÞ¸ÄµØÖ·
											break;
										}
										else if('\r'==*(sSDI12_Para[port].rx_buf+1) && '\n'==*(sSDI12_Para[port].rx_buf+2)){
											 sSDI12_Para[port].rx_ptr = 0;
										}
										else{
											SDI12_DataProcess(port);
											sSDI12_Para[port].rx_ptr = 0;
										} 
										break;
									}
									else{//?
										SDI12_DataProcess(port);
									  sSDI12_Para[port].rx_ptr = 0;
										break;
									}
								}
						}
				sSDI12_Para[port].rx_ptr = 0;
		    eSDI12_BUS[port] = SDI12_IDLE;
    	}
			j = i+1;
    }
		vTaskDelay(30);
		i++;
	}
	SDI12_Data_Ascii_Cnt[port] = 0;
	
	return SDI12_OK;
}
#include "DL212_easy_mode.h" 
void SDI12_DataProcess(unsigned char port){
	unsigned int j=2; 
  
	if(*(sSDI12_Para[port].rx_buf+1)=='\r' ||
		 *(sSDI12_Para[port].rx_buf)==0
	  ){
	  return;
	}
	while(*(sSDI12_Para[port].rx_buf+j) != '\r'){
		if(j >= SDI12_RX_SIZE){
		  return;
		}
		if('+'==*(sSDI12_Para[port].rx_buf+j) || '-'==*(sSDI12_Para[port].rx_buf+j)){
			*(&SDI12_Data_Ascii[port][0]+SDI12_Data_Ascii_Cnt[port]++) = ',';
			*(&SDI12_Data_Ascii[port][0]+SDI12_Data_Ascii_Cnt[port]++) = *(sSDI12_Para[port].rx_buf+j++);
		}
		else{
			*(&SDI12_Data_Ascii[port][0]+SDI12_Data_Ascii_Cnt[port]++)= *(sSDI12_Para[port].rx_buf+j++);
		}
	} 
	if(0 == port){
		*(&SDI12_Data_Ascii[port][0]+SDI12_Data_Ascii_Cnt[port]++) = ',';
	}
}
