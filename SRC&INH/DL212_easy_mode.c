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

float Value[10],Batt,PSW_Value,PLL_Value,C1_Value,C2_Value;
char Value_Ascii[400];
unsigned int Value_Ascii_Len=0; 
unsigned char Value_Count=0,DL212_DebugMode=VALUE_DISPLAY_ON; 
unsigned int RTC_IntCount=0,LastScanIntCount=0;


float Battery(void){
	float bat;
	
	ADS1248SetDataRate(9);
	ADS1248SetGain(0);
	ADS1248SetChannel(2,0);
	ADS1248SetChannel(3,1); 
	delay_us(500);	
	bat = ADS1248Convert(0)/1000;
	bat = bat*251/51; 
	return bat;
}
float VoltDiff(unsigned char chan,unsigned char filter,unsigned char revdiff,unsigned char range){
  float volt,volt_rev,gain,sys_gain=1,offset=0;
	
	switch(filter){
	  case 0:
			ADS1248SetDataRate(2);
		break;
		case 1:
			ADS1248SetDataRate(5); 
		break;
		case 2:
			ADS1248SetDataRate(9);
		break;
		default:
			ADS1248SetDataRate(9); 
		break;
	}
	switch(range){
	  case 0:
			ADS1248SetGain(0);
		  sys_gain=0.99949;
		  offset=-0.2;
			gain = 1;
		break;
		case 1:
			ADS1248SetGain(3);
			gain = 8;
		break;
		case 2:
			ADS1248SetGain(5);
			gain = 32;
		break;
		default:
			gain=1;
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
	delay_us(500);
	volt = ADS1248Convert(range)/gain*sys_gain+offset; 
	if(0 == revdiff){
		switch(chan){
			case 0: 
				ADS1248SetChannel(0,1);
				ADS1248SetChannel(1,0);   
			break;
			case 1: 
				ADS1248SetChannel(4,1);
				ADS1248SetChannel(5,0);
			break;
			case 2: 
				ADS1248SetChannel(6,1);
				ADS1248SetChannel(7,0); 
			break;
		}
		delay_us(500);
    volt_rev = ADS1248Convert(range)/gain*sys_gain+offset; 
	  volt = (volt-volt_rev)/2;
	} 
	
	return volt;
}

float VoltSe(unsigned char chan,unsigned char filter,unsigned char range){
	float volt,gain,sys_gain=1,offset=0;
 
	switch(filter){
	  case 0:
			ADS1248SetDataRate(2);
		break;
		case 1:
			ADS1248SetDataRate(5); 
		break;
		case 2:
			ADS1248SetDataRate(9); 
		break;
		default:
			ADS1248SetDataRate(9); 
		break;
	}
	switch(range){  
	  case 0:
			ADS1248SetGain(0);
		  sys_gain=0.99949;
		  offset=-0.2;
			gain = 1;
		break;
		case 1:
			ADS1248SetGain(3);
		  gain = 8;
		break;
		case 2:
			ADS1248SetGain(5);
		  gain = 32;
		break;
		default:
			gain=1;
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
	delay_us(500); 
  volt = ADS1248Convert(range)/gain*sys_gain+offset; 
	
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
		 Value_Count = Value_Ascii_Len = 0;
		 Batt = Battery(); 
		 Value[Value_Count++] = Batt;
		 if(0 == sDL212_Config.mode[0]){//HL-1
				if(1 == sDL212_Config.sw[0]){
					if(1== sDL212_Config.vx_sw[0]){
					  MCP4725_SetValue(sDL212_Config.vx_value[0]);
						delay_us(20);
					}
				  Value[Value_Count++] = VoltDiff(0,sDL212_Config.filter[0],sDL212_Config.revdiff[0],sDL212_Config.range[0])*sDL212_Config.mul[0]+sDL212_Config.offset[0]; 
				} 
			}
			else{
			  if(1 == sDL212_Config.sw[0]){//H-1
					if(1== sDL212_Config.vx_sw[0]){
					  MCP4725_SetValue(sDL212_Config.vx_value[0]);
						delay_us(20);
					}
				  Value[Value_Count++] = VoltSe(0,sDL212_Config.filter[0],sDL212_Config.range[0])*sDL212_Config.mul[0]+sDL212_Config.offset[0]; 
				}
				if(1 == sDL212_Config.sw[1]){//L-1
					if(1== sDL212_Config.vx_sw[1]){
					  MCP4725_SetValue(sDL212_Config.vx_value[1]);
						delay_us(20);
					}
					Value[Value_Count++] = VoltSe(1,sDL212_Config.filter[1],sDL212_Config.range[1])*sDL212_Config.mul[1]+sDL212_Config.offset[1]; 
				}
			}
	   if(0 == sDL212_Config.mode[1]){//HL-2
				if(1 == sDL212_Config.sw[2]){
					if(1== sDL212_Config.vx_sw[2]){
					  MCP4725_SetValue(sDL212_Config.vx_value[2]);
						delay_us(20);
					}
				  Value[Value_Count++] = VoltDiff(1,sDL212_Config.filter[2],sDL212_Config.revdiff[1],sDL212_Config.range[2])*sDL212_Config.mul[2]+sDL212_Config.offset[2]; 
				} 
			}
			else{
			  if(1 == sDL212_Config.sw[2]){//H-2
					if(1== sDL212_Config.vx_sw[2]){
					  MCP4725_SetValue(sDL212_Config.vx_value[2]);
						delay_us(20);
					}
				  Value[Value_Count++] = VoltSe(2,sDL212_Config.filter[2],sDL212_Config.range[2])*sDL212_Config.mul[2]+sDL212_Config.offset[2]; 
				}
				if(1 == sDL212_Config.sw[3]){//L-2
					if(1== sDL212_Config.vx_sw[3]){
					  MCP4725_SetValue(sDL212_Config.vx_value[3]);
						delay_us(20);
					}
					Value[Value_Count++] = VoltSe(3,sDL212_Config.filter[3],sDL212_Config.range[3])*sDL212_Config.mul[3]+sDL212_Config.offset[3]; 
				}
			}
			if(0 == sDL212_Config.mode[2]){//HL-3
				if(1 == sDL212_Config.sw[4]){
					if(1== sDL212_Config.vx_sw[4]){
					  MCP4725_SetValue(sDL212_Config.vx_value[4]);
						delay_us(20);
					}
				  Value[Value_Count++] = VoltDiff(2,sDL212_Config.filter[4],sDL212_Config.revdiff[2],sDL212_Config.range[4])*sDL212_Config.mul[4]+sDL212_Config.offset[4]; 
				} 
			}
			else{
			  if(1 == sDL212_Config.sw[4]){//H-3
					if(1== sDL212_Config.vx_sw[4]){
					  MCP4725_SetValue(sDL212_Config.vx_value[4]);
						delay_us(20);
					}
				  Value[Value_Count++] = VoltSe(4,sDL212_Config.filter[4],sDL212_Config.range[4])*sDL212_Config.mul[4]+sDL212_Config.offset[4]; 
				}
				if(1 == sDL212_Config.sw[5]){//L-3
					if(1== sDL212_Config.vx_sw[5]){
					  MCP4725_SetValue(sDL212_Config.vx_value[5]);
						delay_us(20);
					}
					Value[Value_Count++] = VoltSe(5,sDL212_Config.filter[5],sDL212_Config.range[5])*sDL212_Config.mul[5]+sDL212_Config.offset[5]; 
				}
			} 
			if(2 != sDL212_Config.sw[6]){//1:常开 0:测量时打开   2：关闭
			  psSW12_Func->sw(0,1);
			} 
			if(sDL212_Config.sw[7]){ 
				Value[Value_Count++] = PSW_Value*sDL212_Config.mul[6]+sDL212_Config.offset[6];  
			}
			if(sDL212_Config.sw[8]){
				Value[Value_Count++] = PLL_Value*sDL212_Config.mul[7]+sDL212_Config.offset[7]; 
			} 
			//把除了SDI12传感器之外的数据先处理成ascii
			Value_Ascii_Len = sprintf(Value_Ascii,"%c%c,",sDL212_Config.device_id[0],sDL212_Config.device_id[1]);  
			for(i=0;i<Value_Count;i++){
			  Value_Ascii_Len += sprintf(Value_Ascii+Value_Ascii_Len,"%.4f,",Value[i]);
			}
			if(1 == sDL212_Config.sw[9]){
			  switch(sDL212_Config.mode[3]){
			    case 0:
						SDI12Recorder(0,(unsigned char*)&sDL212_Config.sdi12_cmd[0][0]);  
					  len = strlen((const char*)&SDI12_Data_Ascii[0][0]);
						if(len > 1){
						  memcpy(Value_Ascii+Value_Ascii_Len,&SDI12_Data_Ascii[0][0],len);
					    Value_Ascii_Len += len;
						} 
					break;
					case 1:
						C1_Value = C1_Value*sDL212_Config.mul[8]+sDL212_Config.offset[8];
						Value_Ascii_Len += sprintf(Value_Ascii+Value_Ascii_Len,"%.4f,",C1_Value);
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
					  if(len > 1){
						  memcpy(Value_Ascii+Value_Ascii_Len,&SDI12_Data_Ascii[1][0],len); 
					    Value_Ascii_Len = Value_Ascii_Len+len;  
						} 
					break;
					case 1:
					  C2_Value = C2_Value*sDL212_Config.mul[9]+sDL212_Config.offset[9];
						Value_Ascii_Len += sprintf(Value_Ascii+Value_Ascii_Len,"%.4f,",C2_Value);
					break;
					default:
					break;
			  }	
			} 
			Value_Ascii_Len += sprintf(Value_Ascii+Value_Ascii_Len,"\r\n");   
		  /*pack = Value_Ascii_Len/ZIGBEE_PACKET_MAX+1;
			for(i=0;i<pack-1;i++){ 
				memcpy(USART_DMA_TxBuf,Value_Ascii+i*ZIGBEE_PACKET_MAX,ZIGBEE_PACKET_MAX);
				DMA_SetCurrDataCounter(DMA1_Channel4,ZIGBEE_PACKET_MAX); 
				DMA_Cmd(DMA1_Channel4,ENABLE); 
				if(xSemaphoreTake(BinarySemaphore_USART,100) == pdTRUE){ 
				  __nop();	
				}
				else{
				  __nop();	
				}
			} 
			memcpy(USART_DMA_TxBuf,Value_Ascii+i*ZIGBEE_PACKET_MAX,Value_Ascii_Len-i*ZIGBEE_PACKET_MAX);  
			DMA_SetCurrDataCounter(DMA1_Channel4,Value_Ascii_Len-i*ZIGBEE_PACKET_MAX); 
			DMA_Cmd(DMA1_Channel4,ENABLE); 
			数据较多时，必须分包，目前设置的是250个字节，不分包*/
			memcpy(USART_DMA_TxBuf,Value_Ascii ,Value_Ascii_Len ); 
			DMA_SetCurrDataCounter(DMA1_Channel4,Value_Ascii_Len ); 
			DMA_Cmd(DMA1_Channel4,ENABLE);
			if(xSemaphoreTake(BinarySemaphore_USART,200) == pdTRUE){ 
			  sUSB_Para.tx_len = Value_Ascii_Len;
			  memcpy(sUSB_Para.tx_buf,Value_Ascii,Value_Ascii_Len); 
			  memset(Value_Ascii,0,400); 
			}
			else{
				len = sprintf((char*)sUSB_Para.tx_buf,"zigbee串口发送数据失败\r\n"); 
			  memcpy(sUSB_Para.tx_buf+len,Value_Ascii,Value_Ascii_Len); 
				sUSB_Para.tx_len = Value_Ascii_Len+len;
			  memset(Value_Ascii,0,400); 	
			}
			
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
	
	SPI_FRAM_SPI_Init();
	SPI_FRAM_ReadBytes(0,(unsigned char*)&sDL212_Config,sizeof(sDL212_Config));
	ADS1248_SPI_Init();
  //EEPROM_Read((unsigned char*)&sDL212_Config,EEPROM_BANK_START_ADDR,sizeof(sDL212_Config));
  p = (char *)&sDL212_Config;
	lrc = LRC((unsigned char*)p,sizeof(sDL212_Config)-4);
	if(lrc!=sDL212_Config.lrc || 0==lrc){//eeprom中不存在配置或者配置数据出错
		sDL212_Config.scan = 0xFFFFFFFF; 
	}
	else{	
		if(sDL212_Config.sw[7]){
			TIM2_TI2FP2_Init();//PSW-----PA1(TIM2_CH2)
			if(sDL212_Config.mea_time[0]){
				TIM6_Init(sDL212_Config.mea_time[0]);
			}
			else{
				TIM6_Init(1000);
			} 
		}
		else{
			TIM_Cmd(TIM6, DISABLE);
		}
		if(sDL212_Config.sw[8]){
			TIM5_TI1FP1_Init();//PLL---PA0(TIM5_CH1) 
			if(sDL212_Config.mea_time[1]){
				TIM10_Init(sDL212_Config.mea_time[1]);
			}
			else{
				TIM10_Init(1000);
			}  
		}	
		else{
			TIM_Cmd(TIM10, DISABLE);
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
}

void DL212_Config_Utility(void){
  unsigned int i=0,timeout=0;
	char message[10],*p;
	static struct CONFIG cfg; 
  unsigned char lrc=0;	
	
	if(0 == strncmp("DL212",(const char*)(sUSB_Para.rx_buf),5)){  
		if(eSuspended != eTaskGetState(Task1_Handler)){
			vTaskSuspend(Task1_Handler); 
		}
	  if(0 == strncmp("DL212 Configuration Utility Write",(const char*)(sUSB_Para.rx_buf),34)){  
			sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0; 		
			p = (char*)&cfg;
			while(i<sizeof(sDL212_Config) && timeout++<0x100000){	
				CDC_Receive_DATA();	
				if(sUSB_Para.packet_rec){ 
					timeout = 0;				
					memcpy(p+i,sUSB_Para.rx_buf,sUSB_Para.rec_len);	 	 
					i += sUSB_Para.rec_len;	 
					sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0; 	
				}	
			} 
			lrc = LRC((unsigned char*)&cfg,sizeof(sDL212_Config)-4);  	
			if(lrc == cfg.lrc){
				i=sprintf(message,"lrc ok\r\n");USB_Send((unsigned char *)message,i);	
				memcpy((char *)(&sDL212_Config),(char *)&cfg,sizeof(sDL212_Config));	
				SPI_FRAM_SPI_Init();
				SPI_FRAM_WriteBytes(0,(unsigned char*)&sDL212_Config,sizeof(sDL212_Config)); 
				ADS1248_SPI_Init();
				//EEPROM_Write((unsigned char*)&sDL212_Config,EEPROM_BANK_START_ADDR,sizeof(sDL212_Config)); 
				DL212_EasyMode_Init();
			}	
			else{	
				i=sprintf(message,"lrc error\r\n");USB_Send((unsigned char *)message,i);	
			}
		}
		else if(0 == strncmp("DL212 Configuration Utility Read",(const char*)(sUSB_Para.rx_buf),33)){ 
			sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0;
			USB_Send((unsigned char*)&sDL212_Config,sizeof(sDL212_Config));
		}
		else if(0 == strncmp("DL212 value display on",(const char*)(sUSB_Para.rx_buf),22)){ 
			sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0;		
		  DL212_DebugMode = VALUE_DISPLAY_ON;	 
		}
		else if(0 == strncmp("DL212 value display off",(const char*)(sUSB_Para.rx_buf),23)){
			sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0;		
		  DL212_DebugMode = VALUE_DISPLAY_OFF;	 
		}
		else if(0 == strncmp("DL212 c1 port sdi12 transparent",(const char*)(sUSB_Para.rx_buf),32)){  
			sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0;
			DL212_DebugMode = SDI12_0_TRANSPARENT; 
		}
		else if(0 == strncmp("DL212 c2 port sdi12 transparent",(const char*)(sUSB_Para.rx_buf),32)){  
			sUSB_Para.packet_rec = 0; sUSB_Para.rec_len = 0;
			DL212_DebugMode = SDI12_1_TRANSPARENT; 
		} 
		else{
		  __nop();
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

