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
#include "ads1248.h"
#include "stm32l1xx_tim.h"

struct CONFIG sDL212_Config;
float Value[7];
unsigned char DL212_EasyMode=1;
unsigned char DL212_Value_Display_Ctrl=0;
unsigned int LastScanIntCount=0;

float Var[200]={0}; 
unsigned int  C1_Dest=0  ,C2_Dest=0  ,P_SW_Dest=0  ,F_Mea_Dest=0;
unsigned int  C1_Time=0  ,C2_Time=0  ,P_SW_Time=0  ,F_Mea_Time=0; 
unsigned char C1_Option=0,C2_Option=0,P_SW_Option=0,F_Mea_Option=0;
 

float Battery(void){
	float bat;
	
	ADS1248SetChannel(2,0);
	ADS1248SetChannel(3,1);  
	bat = (float)ADS1248RDATARead()*3/0x7FFFFF;
	bat = bat*251/51;
	
	return bat;
}

void VoltSe(void){

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

float VoltSE(unsigned char chan,unsigned char range){
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

void P_SW(void){

}

void PLL(void){

}

void D1(void){

}

void DL212_EasyMode_Scan(void){
	unsigned int i=0;
	char message[30];
	
	/*if(sDL212_Config.scan && RTC_M41T81_IntCount!=LastScanIntCount){ 
	  if(0 == RTC_M41T81_IntCount%sDL212_Config.scan){ 
			LastScanIntCount = RTC_M41T81_IntCount; 
	    switch(sDL212_Config.port[0]){ 
			  case 0:
					Value[0] = psSE_FUNC->read(0);
					if(0 == sDL212_Config.mode){
			      printf("%c%c,v1,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[0]); 
					}
				break;
				case 1:
					Value[0] = Var[F_Mea_Dest];
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v1,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[0]);   
					} 
				break;
				case 2:
					Value[0] = 0;
					psSE_FUNC->out_ctrl(0,1);
				break;
				case 3:
					Value[0] = 0;
				  psSE_FUNC->out_ctrl(0,0);
				break;
				default:
				break;
			}
			if(1 == DL212_Value_Display_Ctrl){
		    i=sprintf(message,"v1 value:%.1f\r\n",Value[0]);USB_Send((unsigned char *)message,i); 
			}
			switch(sDL212_Config.port[1]){
			  case 0:
					Value[1] = psSE_FUNC->read(1);
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v2,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[1]); 
					}
				break;
				case 1:
					Value[1] = Var[F_Mea_Dest];
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v2,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[1]); 
					}
				break;
				case 2:
					Value[1] = 0;
					psSE_FUNC->out_ctrl(1,1);
				break;
				case 3:
					Value[1] = 0;
				  psSE_FUNC->out_ctrl(1,0);
				break;
				default:
				break;
			}
			if(1 == DL212_Value_Display_Ctrl){
		    i=sprintf(message,"v2 value:%.1f\r\n",Value[1]);USB_Send((unsigned char *)message,i); 
			}
			switch(sDL212_Config.port[2]){
			  case 0:
					Value[2] = psSE_FUNC->read(2);
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v3,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[2]);
					}
				break;
				case 1:
					Value[2] = Var[F_Mea_Dest];
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v3,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[2]);
					}
				break;
				case 2:
					Value[2] = 0;
					psSE_FUNC->out_ctrl(2,1);
				break;
				case 3:
					Value[2] = 0;
				  psSE_FUNC->out_ctrl(2,0);
				break;
				default:
				break;
			}
			if(1 == DL212_Value_Display_Ctrl){
		    i=sprintf(message,"v3 value:%.1f\r\n",Value[2]);USB_Send((unsigned char *)message,i); 
			}
			switch(sDL212_Config.port[3]){
			  case 0:
					Value[3] = psSE_FUNC->read(3);
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v4,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[3]);
					}
				break;
				case 1:
					Value[3] = Var[F_Mea_Dest];
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v4,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[3]);
					}
				break;
				case 2:
					Value[3] = 0;
					psSE_FUNC->out_ctrl(3,1);
				break;
				case 3:
					Value[3] = 0;
				  psSE_FUNC->out_ctrl(3,0);
				break;
				default:
				break;
			}
			if(1 == DL212_Value_Display_Ctrl){
		    i=sprintf(message,"v4 value:%.1f\r\n",Value[3]);USB_Send((unsigned char *)message,i); 
			}
			if(0 == sDL212_Config.port[4]){
				Value[4] = Var[P_SW_Dest];
				if(0 == sDL212_Config.mode){
				  printf("%c%c,f1,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[4]);
				}
			}
			else{
			  Value[4] = 0;
			}
			if(1 == DL212_Value_Display_Ctrl){
		    i=sprintf(message,"f1 value:%.0f\r\n",Value[4]);USB_Send((unsigned char *)message,i); 
			}
			switch(sDL212_Config.port[5]){
			  case 0:
					Value[5] = Var[C1_Dest];
				  if(0 == sDL212_Config.mode){
				    printf("%c%c,d1,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[5]);
				  }
				  if(1 == DL212_Value_Display_Ctrl){
		        i=sprintf(message,"d1 value:%.0f\r\n",Value[5]);USB_Send((unsigned char *)message,i); 
			    } 
				break;
				case 1:
					SDI12Recorder(0,(unsigned char*)&sDL212_Config.sdi12[0][0]);
				break;
				case 2: 
					psC_OUT_Func->out(0,1);
				break;
				case 3:
					Value[5] = 0;
				  psC_OUT_Func->out(0,0);
				break;
				default:
				break;
			}
			switch(sDL212_Config.port[6]){
			  case 0:
					Value[6] = Var[C2_Dest];
				  if(0 == sDL212_Config.mode){
				    printf("%c%c,d2,%.1f\r\n",sDL212_Config.device_id[0],sDL212_Config.device_id[1],Value[6]);
				  }
				  if(1 == DL212_Value_Display_Ctrl){
		        i=sprintf(message,"d2 value:%.0f\r\n\r\n",Value[6]);USB_Send((unsigned char *)message,i); 
		    	} 
				break;
				case 1:
					SDI12Recorder(1,(unsigned char*)&sDL212_Config.sdi12[1][0]);
				break;
				case 2: 
					psC_OUT_Func->out(1,1);
				break;
				case 3:
					Value[6] = 0;
				  psC_OUT_Func->out(1,0);
				break;
				default:
				break;
			} 
			//if(0 == sDL212_Config.mode){
		    //printf("%c%c,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[0],Value[1],Value[2],Value[3],Value[4],Value[5],Value[6]);
			//}
	  }
	} */
} 

void DL212_EasyMode_ValueDisplay(void){
	unsigned char i; 
	char message[30]; 
	
  if(0 == strncmp("value display on",(const char*)(sUSB_Para.rx_buf),16)){
	  DL212_Value_Display_Ctrl = 1;
  }
  else if(0 == strncmp("value display off",(const char*)(sUSB_Para.rx_buf),17)){
	  DL212_Value_Display_Ctrl = 0;
	}
}

void DL212_EasyMode_Init(void){
  EEPROM_Read((unsigned char*)&sDL212_Config,EEPROM_BANK_START_ADDR,sizeof(sDL212_Config));
 
		/*P_SW_Time  =C1_Time  =C2_Time  =F_Mea_Time  =1000;
		P_SW_Option=C1_Option=C2_Option=F_Mea_Option=1;
	  P_SW_Dest=0,C1_Dest=1,C2_Dest=2,F_Mea_Dest=3;*/
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
	if(1 == sDL212_Config.sw[6]){//sw12常开
		psSW12_Func->sw(1,1);
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

