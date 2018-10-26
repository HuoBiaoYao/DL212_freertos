#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "my_usb.h"
#include "hw.h"
#include "hal.h"
#include "delay.h"
#include "ctype.h" 
#include "DL212_easy_mode.h"
#include "main.h"
#include "mcp4725.h"
#include "ads1248.h"
#include "stm32l1xx_tim.h"

struct CONFIG sDL212_Config;

float Value[8],PSW_Value,PLL_Value,C1_Value,C2_Value;
char Value_Ascii[400];
unsigned int Value_Ascii_Len=0;
unsigned char Value_Count=0,DL212_DebugMode=VALUE_DISPLAY_ON; 
unsigned int RTC_IntCount=0,LastScanIntCount=0;


float Battery(void){
	float bat;
	
	ADS1248SetChannel(2,0);
	ADS1248SetChannel(3,1);  
	bat = (float)ADS1248ReadDATA()*3/0x7FFFFF;
	bat = bat*251/51;
	vTaskDelay(1); 
	return bat;
}
float VoltDiff(unsigned char chan,unsigned char range){
	float volt;
	ADS1248_SPI_SendByte(ADS1248_CMD_SDATAC); 
	switch(range){
	  case 0:
			ADS1248SetGain(0);
		break;
		case 1:
			ADS1248SetGain(3);
		break;
		case 2:
			ADS1248SetGain(5);
		break;
		default:
		break;
	}
	switch(chan){
	  case 0: 
			ADS1248SetChannel(0,0);
			ADS1248SetChannel(1,1);   
		break;
		case 1: 
			ADS1248SetChannel(4,0);
			ADS1248SetChannel(5,1);
		break;
		case 2: 
			ADS1248SetChannel(6,0);
			ADS1248SetChannel(7,1); 
		break;
	} 
  volt = (float)ADS1248ReadDATA()*3/0x7FFFFF; 
	vTaskDelay(1); 
	return volt;
}

float VoltSe(unsigned char chan,unsigned char range){
	float volt;
	 ADS1248_SPI_SendByte(ADS1248_CMD_SDATAC); ADS1248_SPI_SendByte(ADS1248_CMD_NOP);
	switch(range){  
	  case 0:
			ADS1248SetGain(0);
		break;
		case 1:
			ADS1248SetGain(3);
		break;
		case 2:
			ADS1248SetGain(5);
		break;
		default:
		break;
	}
	ADS1248SetChannel(3,1);
	switch(chan){
	  case 0:
			ADS1248SetChannel(0,0); 
		break;
		case 1:
			ADS1248SetChannel(1,0);
		break;
		case 2:
			ADS1248SetChannel(4,0);   
		break;
		case 3:
			ADS1248SetChannel(5,0);   
		break;
		case 4:
			ADS1248SetChannel(6,0);   
		break;
		case 5:
			ADS1248SetChannel(7,0);
		break;
		default:
		break;
	}
  volt = (float)ADS1248ReadDATA()*3/0x7FFFFF;
	vTaskDelay(1); 
	
	return volt;
} 

void DL212_EasyMode_Scan(void){
	unsigned int len,i,pack; 
	int timeout=100000;
//#ifdef USE_PCF8563 	
	//if(sDL212_Config.scan && RTC_IntCount != LastScanIntCount){ 
	  //if(0 == RTC_IntCount%sDL212_Config.scan){ 
			//LastScanIntCount = RTC_IntCount; 
//#endif
			Value_Count = 0;
			Value_Ascii_Len = 0;
		 //ADS1248_SPI_SendByte(ADS1248_CMD_WAKEUP);
		 if(0 == sDL212_Config.mode[0]){//HL-1
				if(1 == sDL212_Config.sw[0]){
					if(1== sDL212_Config.vx_sw[0]){
					  MCP4725_SetValue(sDL212_Config.vx_value[0]);
					}
				  Value[Value_Count++] = VoltDiff(0,sDL212_Config.range[0]); 
				} 
			}
			else{
			  if(1 == sDL212_Config.sw[0]){//H-1
					if(1== sDL212_Config.vx_sw[0]){
					  MCP4725_SetValue(sDL212_Config.vx_value[0]);
					}
				  Value[Value_Count++] = VoltSe(0,sDL212_Config.range[0]); 
				}
				if(1 == sDL212_Config.sw[1]){//L-1
					if(1== sDL212_Config.vx_sw[1]){
					  MCP4725_SetValue(sDL212_Config.vx_value[1]);
					}
					Value[Value_Count++] = VoltSe(1,sDL212_Config.range[1]); 
				}
			}
	   if(0 == sDL212_Config.mode[1]){//HL-2
				if(1 == sDL212_Config.sw[2]){
					if(1== sDL212_Config.vx_sw[2]){
					  MCP4725_SetValue(sDL212_Config.vx_value[2]);
					}
				  Value[Value_Count++] = VoltDiff(0,sDL212_Config.range[2]); 
				} 
			}
			else{
			  if(1 == sDL212_Config.sw[2]){//H-2
					if(1== sDL212_Config.vx_sw[2]){
					  MCP4725_SetValue(sDL212_Config.vx_value[2]);
					}
				  Value[Value_Count++] = VoltSe(2,sDL212_Config.range[2]); 
				}
				if(1 == sDL212_Config.sw[3]){//L-2
					if(1== sDL212_Config.vx_sw[3]){
					  MCP4725_SetValue(sDL212_Config.vx_value[3]);
					}
					Value[Value_Count++] = VoltSe(3,sDL212_Config.range[3]); 
				}
			}
			if(0 == sDL212_Config.mode[2]){//HL-3
				if(1 == sDL212_Config.sw[4]){
					if(1== sDL212_Config.vx_sw[4]){
					  MCP4725_SetValue(sDL212_Config.vx_value[4]);
					}
				  Value[Value_Count++] = VoltDiff(0,sDL212_Config.range[4]); 
				} 
			}
			else{
			  if(1 == sDL212_Config.sw[4]){//H-3
					if(1== sDL212_Config.vx_sw[4]){
					  MCP4725_SetValue(sDL212_Config.vx_value[4]);
					}
				  Value[Value_Count++] = VoltSe(4,sDL212_Config.range[4]); 
				}
				if(1 == sDL212_Config.sw[5]){//L-3
					if(1== sDL212_Config.vx_sw[5]){
					  MCP4725_SetValue(sDL212_Config.vx_value[5]);
					}
					Value[Value_Count++] = VoltSe(5,sDL212_Config.range[5]); 
				}
			}
			//ADS1248_SPI_SendByte(ADS1248_CMD_SLEEP);
			if(2 != sDL212_Config.sw[6]){//0:常开  1:测量时打开   2：关闭
			  psSW12_Func->sw(0,1);
			} 
			if(sDL212_Config.sw[7]){ 
				Value[Value_Count++] = PSW_Value;  
			}
			if(sDL212_Config.sw[8]){
				Value[Value_Count++] = PLL_Value; 
			} 
			//把除了SDI12传感器之外的数据先处理成ascii
			Value_Ascii_Len = sprintf(Value_Ascii,"%c%c,",sDL212_Config.device_id[0],sDL212_Config.device_id[1]);  
			for(i=0;i<Value_Count;i++){
			  Value_Ascii_Len += sprintf(Value_Ascii+Value_Ascii_Len,"%.3f,",Value[i]);
			}
			if(1 == sDL212_Config.sw[9]){
			  switch(sDL212_Config.mode[3]){
			    case 0:
						SDI12Recorder(0,(unsigned char*)&sDL212_Config.sdi12_cmd[0][0]);  
					  len = strlen((const char*)&SDI12_Data_Ascii[0][0]);
						memcpy(Value_Ascii+Value_Ascii_Len,&SDI12_Data_Ascii[0][0],len);
					  Value_Ascii_Len += len;
					break;
					case 1:
						//C1_Value;
						Value_Ascii_Len += sprintf(Value_Ascii+Value_Ascii_Len,"%.3f,",C1_Value);
					break;
					default:
					break;
			  } 
			} 
			if(1 == sDL212_Config.sw[10]){
			  switch(sDL212_Config.mode[4]){
			    case 0:
						SDI12Recorder(1,(unsigned char*)&sDL212_Config.sdi12_cmd[1][0]);
					  len = strlen((const char*)&SDI12_Data_Ascii[1][0]);
						strcpy(Value_Ascii+Value_Ascii_Len,&SDI12_Data_Ascii[1][0]); 
					  Value_Ascii_Len = Value_Ascii_Len+len+1;
					break;
					case 1:
					  //C2_Value;
						Value_Ascii_Len += sprintf(Value_Ascii+Value_Ascii_Len,"%.3f,",C2_Value);
					break;
					default:
					break;
			  }	
			} 
			Value_Ascii_Len += sprintf(Value_Ascii+Value_Ascii_Len,"\r\n"); 
			 
			pack = Value_Ascii_Len/50+1;
			for(i=0;i<pack-1;i++){
				USART1_DMA_Send_State = 1; 
				memcpy(USART_DMA_TxBuf,Value_Ascii+i*50,50);
				DMA_SetCurrDataCounter(DMA1_Channel4,50); 
				DMA_Cmd(DMA1_Channel4,ENABLE); 
				while(USART1_DMA_Send_State && timeout-->0); 
				if(0 == timeout){__NOP();}
			}
			USART1_DMA_Send_State = 1; 
			memcpy(USART_DMA_TxBuf,Value_Ascii+i*50,Value_Ascii_Len-i*50);
			DMA_SetCurrDataCounter(DMA1_Channel4,Value_Ascii_Len-i*50); 
			DMA_Cmd(DMA1_Channel4,ENABLE); 
			while(USART1_DMA_Send_State && timeout-->0); 
      if(0 == timeout){__NOP();}
      //printf("%s",Value_Ascii+Value_Ascii_Len);  
//#ifdef USE_PCF8563 	
//	  }
//	}
//#endif 
	if(1 != sDL212_Config.sw[6]){
	  psSW12_Func->sw(0,0);
	}
} 


void DL212_EasyMode_Init(void){
	unsigned char lrc=0;
	char *p;
	
  EEPROM_Read((unsigned char*)&sDL212_Config,EEPROM_BANK_START_ADDR,sizeof(sDL212_Config));
  p = (char *)&sDL212_Config;
	lrc = LRC((unsigned char*)p,sizeof(sDL212_Config)-4);
	if(lrc!=sDL212_Config.lrc || 0==lrc){//eeprom中不存在配置或者配置数据出错
    sDL212_Config.device_id[0] = 'C'; 
		sDL212_Config.device_id[1] = 'N';
		sDL212_Config.scan = 1000;
		DL212_DebugMode = VALUE_DISPLAY_ON;
		memset(&sDL212_Config.sw[0],1,11);
		sDL212_Config.sw[6] = 1;//sw12常开
		sDL212_Config.mode[3] = 1;//c1 for psw
		sDL212_Config.mode[4] = 1;//c2 for pll
	} 
	if(sDL212_Config.sw[7]){
		TIM2_TI2FP2_Init();//PSW-----PA1(TIM2_CH2)
		if(sDL212_Config.mea_time[0]){
			TIM10_Init(sDL212_Config.mea_time[0]);
		}
		else{
			TIM10_Init(1000);
		} 
	}
	else{
		TIM_Cmd(TIM10, DISABLE);
	}
	if(sDL212_Config.sw[8]){
		TIM5_TI1FP1_Init();//PLL---PA0(TIM5_CH1) 
		if(sDL212_Config.mea_time[1]){
			TIM6_Init(sDL212_Config.mea_time[1]);
		}
		else{
			TIM6_Init(1000);
		}  
	}	
	else{
	  TIM_Cmd(TIM6, DISABLE);
	}
	switch(sDL212_Config.mode[3]){
	  case 0:
			psSDI12_Func->init(0);
		  TIM_Cmd(TIM7, DISABLE);
		break;
		case 1:
			psC_Pulse_Func->init(0);//C1脉冲测量 
		  TIM9_TI2FP2_Init();//C1------PA3(TIM9_CH2) 
		  if(sDL212_Config.mea_time[2]){
				TIM7_Init(sDL212_Config.mea_time[2]);
	  	}
			else{
				TIM7_Init(1000);
			} 
		break;
		case 2: 
		break;
		default:
		break;
	}
	switch(sDL212_Config.mode[4]){
	  case 0:
			psSDI12_Func->init(1);
			TIM_Cmd(TIM4, DISABLE);
		break;
		case 1:
			psC_Pulse_Func->init(1);//C2脉冲测量  
		  TIM3_ETR_Init();   //C2------PD2(TIM3_ETR)
		  if(sDL212_Config.mea_time[3]){
				TIM4_Init(sDL212_Config.mea_time[3]);
			}
			else{
			  TIM4_Init(1000);
			} 
		break;
		case 2: 
		break;
		default:
		break;
	} 
}

void DL212_Config_Utility(void){
  unsigned int i=0,timeout=0;
	char message[10],*p;
  unsigned char lrc=0;	
	
	if(0 == strncmp("DL212",(const char*)(sUSB_Para.rx_buf),5)){  
	  if(0 == strncmp("DL212 Configuration Utility Write",(const char*)(sUSB_Para.rx_buf),34)){  
			sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0;
			p = (char *)&sDL212_Config;		
			while(i<sizeof(sDL212_Config) && timeout++<0x100000){	
				CDC_Receive_DATA();	
				if(sUSB_Para.packet_rec){ 
					timeout = 0;				
					memcpy((char *)(p+i),sUSB_Para.rx_buf,sUSB_Para.rec_len);	 	 
					i += sUSB_Para.rec_len;	 
					sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0; 	
				}	
			} 
			lrc = LRC((unsigned char*)p,sizeof(sDL212_Config)-4);  	
			if(lrc == sDL212_Config.lrc){
				i=sprintf(message,"lrc ok\r\n");USB_Send((unsigned char *)message,i);	
				EEPROM_Write((unsigned char*)&sDL212_Config,EEPROM_BANK_START_ADDR,sizeof(sDL212_Config)); 
				DL212_EasyMode_Init();
			}	
			else{	
				i=sprintf(message,"lrc error\r\n");USB_Send((unsigned char *)message,i);	
			}
		}
		else if(0 == strncmp("DL212 Configuration Utility Read",(const char*)(sUSB_Para.rx_buf),33)){ 
			sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0;
			p = (char *)&sDL212_Config;		
			USB_Send((unsigned char*)&sDL212_Config,sizeof(sDL212_Config));
		}
		else if(0 == strncmp("DL212 value display on",(const char*)(sUSB_Para.rx_buf),22)){  
		  DL212_DebugMode = VALUE_DISPLAY_ON;	
		}
		else if(0 == strncmp("DL212 value display off",(const char*)(sUSB_Para.rx_buf),23)){  
		  DL212_DebugMode = VALUE_DISPLAY_OFF;	
		}
		else if(0 == strncmp("DL212 c1 port sdi12 transparent",(const char*)(sUSB_Para.rx_buf),32)){  
			DL212_DebugMode = SDI12_0_TRANSPARENT;
		}
		else if(0 == strncmp("DL212 c2 port sdi12 transparent",(const char*)(sUSB_Para.rx_buf),32)){  
			DL212_DebugMode = SDI12_1_TRANSPARENT;
		}
		else if(0 == strncmp("DL212 os version",(const char*)(sUSB_Para.rx_buf),16)){
		  i=sprintf(message,"v1.0.0\r\n");USB_Send((unsigned char *)message,i);		 
		}
	} 
}

unsigned char LRC( unsigned char *buf,unsigned short int len){
  unsigned char lrc=0;

  while(len--){
    lrc += *buf++;
  }  
  lrc = (unsigned char)(-((char)lrc));
	
  return lrc;
}

