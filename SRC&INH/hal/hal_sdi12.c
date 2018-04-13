#include "string.h" 
#include "hal.h"
#include "hw.h"  
#include "delay.h"
#include "my_usb.h"   
#include "stm32l1xx_gpio.h"

__SDI12_BUS eSDI12_BUS[2];
struct _SDI12_PARA sSDI12_Para[2];
struct _SDI12_FUNC sSDI12_Func={
  SDI12_Init,
  SDI12_Transparent,
  SDI12_Read,
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
	
	eC_PORT_STATE[port] = C_SDI12_IN_USE;
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
	if(C_SDI12_IN_USE != eC_PORT_STATE[port]){
	  return SDI12_NOT_IN_USE;
	}
  if(SDI12_REICEIVE == eSDI12_BUS[port]){
	  *(dst+sSDI12_Para[port].rx_ptr++) = (*src&0x7F);
		if(sSDI12_Para[port].rx_ptr >= SDI12_RX_SIZE){
		  sSDI12_Para[port].rx_ptr = 0;
		}
	}
	
	return SDI12_OK;
}
 
__SDI12_RSL SDI12_Send(unsigned char port,unsigned char *string,unsigned int len){
	unsigned int i=0;
	
	if(C_SDI12_IN_USE != eC_PORT_STATE[port]){
	  return SDI12_NOT_IN_USE;
	}
	eSDI12_BUS[port] = SDI12_SEND;
	psSDI12_Func->send_enable(port);
	switch(port){
	  case 0:
			USART_Cmd(USART3,DISABLE);
		break;
		case 1:
			USART_Cmd(UART5,DISABLE);
		break;
		default: 
		break;
	}
	psSDI12_Func->bus_high(port);
	delay_ms(13);
	psSDI12_Func->bus_low(port);
	switch(port){
	  case 0:
			USART_Cmd(USART3,ENABLE);
		break;
		case 1:
			USART_Cmd(UART5,ENABLE);
		break;
		default: 
		break;
	}
	delay_ms(9); 
	while(i < len){
		psSDI12_Func->sendbyte(port,string+i),i++;
	} 
	psSDI12_Func->send_disable(port); 
	eSDI12_BUS[port] = SDI12_BUSY;
	
	return SDI12_OK;
} 

__SDI12_RSL SDI12_SendByte(unsigned char port,unsigned char *byte){
	unsigned char i,even=0;
	 
	if(C_SDI12_IN_USE != eC_PORT_STATE[port]){
	  return SDI12_NOT_IN_USE;
	}
	psSDI12_Func->start(port);
	delay_us(DElAY_Baud1200);
	for(i=0;i<7;i++){
		if(*byte&0x01){
			psSDI12_Func->bus_low(port);
			delay_us(DElAY_Baud1200);
		  even++;
		}
		else{
		  psSDI12_Func->bus_high(port);
			delay_us(DElAY_Baud1200);
		}
		*byte >>= 1;
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
	unsigned int i,timeout;
	static  int reccnt=0;
	
	if(C_SDI12_IN_USE != eC_PORT_STATE[port]){
	  return SDI12_NOT_IN_USE;
	}
  if(bDeviceState == CONFIGURED){
    CDC_Receive_DATA();
    if(sUSB_Para.packet_rec){
      sUSB_Para.packet_rec = 0;
      memcpy(sSDI12_Para[port].tx_buf+sSDI12_Para[port].tx_ptr,sUSB_Para.rx_buf,sUSB_Para.rec_len);  
      sSDI12_Para[port].tx_ptr = sSDI12_Para[port].tx_ptr>=SDI12_TX_SIZE?0:(sSDI12_Para[port].tx_ptr+sUSB_Para.rec_len); 
      sUSB_Para.rec_len = sUSB_Para.rec_ptr = 0;
			reccnt++;
    }
	} 
	if(sSDI12_Para[port].tx_buf[0]=='?' || 
		(sSDI12_Para[port].tx_buf[0]>=0x30 && sSDI12_Para[port].tx_buf[0]<=0x39) ||
		(sSDI12_Para[port].tx_buf[0]>=0x41 && sSDI12_Para[port].tx_buf[0]<=0x5A) ||  
    (sSDI12_Para[port].tx_buf[0]>=0x61 && sSDI12_Para[port].tx_buf[0]<=0x7A)){
	  if(sSDI12_Para[port].tx_ptr >= 2){
	    for(i=0;i<sSDI12_Para[port].tx_ptr;i++){
	      if('!' == sSDI12_Para[port].tx_buf[i-1] && '\r' == sSDI12_Para[port].tx_buf[i]){
	        sSDI12_Para[port].tx_buf[i] = 0;
	        if(SDI12_IDLE != eSDI12_BUS[port]){
	          return SDI12_OK;
	        }
	        eSDI12_BUS[port] = SDI12_BUSY;
					psSDI12_Func->send(port,sSDI12_Para[port].tx_buf,i);
	        delay_ms(8);
	        eSDI12_BUS[port] = SDI12_REICEIVE;
	        while(timeout <= 810){
	          if(0x0D==*(sSDI12_Para[port].rx_buf+sSDI12_Para[port].rx_ptr-2) && 0x0A==*(sSDI12_Para[port].rx_buf+sSDI12_Para[port].rx_ptr-1)){
	            i = sSDI12_Para[port].rx_ptr;
	            sSDI12_Para[port].rx_ptr = 0;
	            break;
	          }
	          delay_ms(1);
	          timeout++;
	        }
	        sSDI12_Para[port].rx_ptr = 0;
	        eSDI12_BUS[port] = SDI12_IDLE;
	        if(i >= 2){
	          if(0x0D==*(sSDI12_Para[port].rx_buf+i-2) && 0x0A==*(sSDI12_Para[port].rx_buf+i-1)){
	            USB_Send(sSDI12_Para[port].rx_buf,i);
	          }
	        }
	        i = 0;
	        break;
	      }
      } 	
	  }
	  sSDI12_Para[port].tx_buf[0] = sSDI12_Para[port].tx_ptr = 0;	 
	}
  else{
		sSDI12_Para[port].tx_buf[0] = 0;
		sSDI12_Para[port].tx_ptr = 0;
  }
	
	return SDI12_OK;
}
    


