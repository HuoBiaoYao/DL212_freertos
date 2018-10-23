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
unsigned char DL212_DebugMode=VALUE_DISPLAY;
float PSW_Value,PLL_Value,C1_Value,C2_Value;
float Value[80];
unsigned char Value_Count=0; 
unsigned int RTC_IntCount=0,LastScanIntCount=0;


float Battery(void){
	float bat;
	
	ADS1248SetChannel(2,0);
	ADS1248SetChannel(3,1);  
	bat = (float)ADS1248RDATARead()*3/0x7FFFFF;
	bat = bat*251/51;
	
	return bat;
}
float VoltDiff(unsigned char chan,unsigned char range){
	float volt;
	
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
  volt = (float)ADS1248RDATARead()*3/0x7FFFFF;
  vTaskDelay(1); 
	
	return volt;
}

float VoltSe(unsigned char chan,unsigned char range){
	float volt;
	
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
			ADS1248SetChannel(3,1);   
		break;
		case 1:
			ADS1248SetChannel(1,0);
			ADS1248SetChannel(3,1);   
		break;
		case 2:
			ADS1248SetChannel(4,0);
			ADS1248SetChannel(3,1);   
		break;
		case 3:
			ADS1248SetChannel(5,0);
			ADS1248SetChannel(3,1);   
		break;
		case 4:
			ADS1248SetChannel(6,0);
			ADS1248SetChannel(3,1);   
		break;
		case 5:
			ADS1248SetChannel(7,0);
			ADS1248SetChannel(3,1);   
		break;
		default:
		break;
	}
  volt = (float)ADS1248RDATARead()*3/0x7FFFFF;
  vTaskDelay(3); 
	
	return volt;
} 

void DL212_EasyMode_Scan(void){
	unsigned int i=0;
	char message[30];
	
	if(sDL212_Config.scan && RTC_IntCount != LastScanIntCount){ 
	  if(0 == RTC_IntCount%sDL212_Config.scan){ 
			LastScanIntCount = RTC_IntCount; 
			Value_Count = 0;
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
				  Value[Value_Count++] = VoltSe(0,sDL212_Config.range[2]); 
				}
				if(1 == sDL212_Config.sw[3]){//L-2
					if(1== sDL212_Config.vx_sw[3]){
					  MCP4725_SetValue(sDL212_Config.vx_value[3]);
					}
					Value[Value_Count++] = VoltSe(1,sDL212_Config.range[3]); 
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
				  Value[Value_Count++] = VoltSe(0,sDL212_Config.range[4]); 
				}
				if(1 == sDL212_Config.sw[5]){//L-3
					if(1== sDL212_Config.vx_sw[5]){
					  MCP4725_SetValue(sDL212_Config.vx_value[5]);
					}
					Value[Value_Count++] = VoltSe(1,sDL212_Config.range[5]); 
				}
			}
			if(2 != sDL212_Config.sw[6]){//0:常开  1:测量时打开   2：关闭
					psSW12_Func->sw(1,1);
			}
			if(sDL212_Config.sw[7]){ 
				Value[Value_Count++] = PSW_Value;  
			}
			if(0 == sDL212_Config.sw[8]){
				Value[Value_Count++] = PLL_Value; 
			} 
			if(1 == sDL212_Config.sw[9]){
			  switch(sDL212_Config.mode[3]){
			    case 0:
						SDI12Recorder(0,(unsigned char*)&sDL212_Config.sdi12_cmd[0][0]); ;
					break;
					case 1:
						Value[Value_Count++] = C1_Value;
					break;
					default:
					break;
			  } 
			} 
			if(1 == sDL212_Config.sw[10]){
			  switch(sDL212_Config.mode[4]){
			    case 0:
						SDI12Recorder(1,(unsigned char*)&sDL212_Config.sdi12_cmd[0][0]);
					break;
					case 1:
					  Value[Value_Count++] = C2_Value;
					break;
					default:
					break;
			  }	
			}
			//if(0 == strncmp("value display on",(const char*)(sUSB_Para.rx_buf),16)){
			//printf("%c%c,d1,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[5]); 
			//i=sprintf(message,"v1 value:%.1f\r\n",Value[0]);USB_Send((unsigned char *)message,i);  
			//if(0 == sDL212_Config.mode){
		  //printf("%c%c,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[0],Value[1],Value[2],Value[3],Value[4],Value[5],Value[6]);
			//}
	  }
	}
} 
  
void DL212_EasyMode_Init(void){
  EEPROM_Read((unsigned char*)&sDL212_Config,EEPROM_BANK_START_ADDR,sizeof(sDL212_Config));
 
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
			;
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
			;
		break;
		default:
		break;
	} 
}

void DL212_Config_Utility(void){
  unsigned int i=0,timeout=0;
	char message[10],*p;
  unsigned char lrc=0;	
	
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
}

unsigned char LRC( unsigned char *buf,unsigned short int len){
  unsigned char lrc=0;

  while(len--){
    lrc += *buf++;
  }  
  lrc = (unsigned char)(-((char)lrc));
	
  return lrc;
}

