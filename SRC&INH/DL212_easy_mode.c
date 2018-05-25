#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "my_usb.h"
#include "hw.h"
#include "hal.h"
#include "delay.h"
#include "ctype.h"
#include "DL212.h"
#include "DL212_easy_mode.h"
#include "main.h"

struct _SPECIALDATA sEMData;
unsigned char DL212_EasyMode=1;
unsigned char DL212_Value_Display_Ctrl=0;
unsigned int LastScanIntCount=0;

void DL212_EasyMode_Scan(void){
	unsigned int i=0;
	char message[30];
	
	DL212_EasyMode_ValueDisplay();
	if(sEMData.scan_intvl && RTC_M41T81_IntCount!=LastScanIntCount){ 
	  if(0 == RTC_M41T81_IntCount%sEMData.scan_intvl){ 
			LastScanIntCount = RTC_M41T81_IntCount; 
	    switch(sEMData.v1_func){ 
			  case 0:
					sEMData.value[i++] = psSE_FUNC->read(0);
				break;
				case 1:
					sEMData.value[i++] = Var[F_Mea_Dest];
				break;
				case 2:
					sEMData.value[i++] = 0;
					psSE_FUNC->out_ctrl(0,1);
				break;
				default:
				break;
			}
			switch(sEMData.v2_func){
			  case 0:
					sEMData.value[i++] = psSE_FUNC->read(1);
				break;
				case 1:
					sEMData.value[i++] = Var[F_Mea_Dest];
				break;
				case 2:
					sEMData.value[i++] = 0;
					psSE_FUNC->out_ctrl(1,1);
				break;
				default:
				break;
			}
			switch(sEMData.v3_func){
			  case 0:
					sEMData.value[i++] = psSE_FUNC->read(2);
				break;
				case 1:
					sEMData.value[i++] = Var[F_Mea_Dest];
				break;
				case 2:
					sEMData.value[i++] = 0;
					psSE_FUNC->out_ctrl(2,1);
				break;
				default:
				break;
			}
			switch(sEMData.v4_func){
			  case 0:
					sEMData.value[i++] = psSE_FUNC->read(3);
				break;
				case 1:
					sEMData.value[i++] = Var[F_Mea_Dest];
				break;
				case 2:
					sEMData.value[i++] = 0;
					psSE_FUNC->out_ctrl(3,1);
				break;
				default:
				break;
			}
			if(0 == sEMData.f1_func){
			  sEMData.value[i++] = Var[P_SW_Dest];
			}
			switch(sEMData.d1_func){
			  case 0:
					sEMData.value[i++] = Var[C1_Dest];
				break;
				case 1:
					sEMData.value[i++] = 0;
				break;
				case 2:
					sEMData.value[i++] = 0;
					psC_OUT_Func->out(0,1);
				break;
				default:
				break;
			}
			switch(sEMData.d2_func){
			  case 0:
					sEMData.value[i++] = Var[C2_Dest];
				break;
				case 1:
					sEMData.value[i++] = 0;
				break;
				case 2:
					sEMData.value[i++] = 0;
					psC_OUT_Func->out(1,1);
				break;
				default:
				break;
			}
			if(1 == sEMData.sw12_1_func){
			  psSW12_Func->sw(0,1);
			}
			if(1 == sEMData.sw12_2_func){
			  psSW12_Func->sw(1,1);
			}
			if(1 == DL212_Value_Display_Ctrl){
		    i=sprintf(message,"v1 value:%.1f\r\n",sEMData.value[0]);USB_Send((unsigned char *)message,i); 
        i=sprintf(message,"v2 value:%.1f\r\n",sEMData.value[1]);USB_Send((unsigned char *)message,i); 
	 	    i=sprintf(message,"v3 value:%.1f\r\n",sEMData.value[2]);USB_Send((unsigned char *)message,i); 
		    i=sprintf(message,"v4 value:%.1f\r\n",sEMData.value[3]);USB_Send((unsigned char *)message,i); 
		    i=sprintf(message,"f1 value:%.0f\r\n",sEMData.value[4]);USB_Send((unsigned char *)message,i); 
		    i=sprintf(message,"d1 value:%.0f\r\n",sEMData.value[5]);USB_Send((unsigned char *)message,i); 
		    i=sprintf(message,"d2 value:%.0f\r\n\r\n",sEMData.value[6]);USB_Send((unsigned char *)message,i); 
		  } 
			if(0 == sEMData.usart_mode){
		    printf("%c%c,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\r\n",sEMData.addr_0,sEMData.addr_1,sEMData.value[0],sEMData.value[1],sEMData.value[2],sEMData.value[3],sEMData.value[4],sEMData.value[5],sEMData.value[6]);
			}
	  }
	} 
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
  EEPROM_Read((unsigned char*)&sEMData,EEPROM_BANK_START_ADDR,sizeof(sEMData));
	if(1 == DL212_EasyMode){ 
		P_SW_Time  =C1_Time  =C2_Time  =F_Mea_Time  =1000;
		P_SW_Option=C1_Option=C2_Option=F_Mea_Option=1;
	  P_SW_Dest=0,C1_Dest=1,C2_Dest=2,F_Mea_Dest=3;
		switch(sEMData.v1_func){ 
      case 1: 
				psSE_FUNC->chan_open(0);
        TIM10_Init(1000);
      break;
      case 2: 
        TIM_Cmd(TIM10, DISABLE);
      break;
      default:
      break;
    }
	  switch(sEMData.v2_func){ 
      case 1: 
				psSE_FUNC->chan_open(1);
        TIM10_Init(1000);
      break;
      case 2: 
        TIM_Cmd(TIM10, DISABLE);
      break;
      default:
      break;
    }
	  switch(sEMData.v3_func){ 
      case 1: 
				psSE_FUNC->chan_open(2);
        TIM10_Init(1000);
      break;
      case 2: 
        TIM_Cmd(TIM10, DISABLE);
      break;
      default:
      break;
    }
	  switch(sEMData.v4_func){ 
      case 1: 
				psSE_FUNC->chan_open(3);
        TIM10_Init(1000);
      break;
      case 2: 
        TIM_Cmd(TIM10, DISABLE);
      break;
      default:
      break;
    }
	  switch(sEMData.f1_func){
	    case 0: 
	      TIM6_Init(1000);
	    break;
	    case 1: 
	      TIM_Cmd(TIM6,DISABLE);;
	    break;  
	  }
	  switch(sEMData.d1_func){
	    case 0: 
	      psC_Pulse_Func->init(0);
	      TIM7_Init(1000); 
	    break;
	    case 1: 
	    break;
	    case 2: 
	      TIM_Cmd(TIM7, DISABLE);
	      psC_OUT_Func->init(0);
	    break;
	  }
	  switch(sEMData.v2_func){
	    case 0: 
	      psC_Pulse_Func->init(1);
	      TIM4_Init(1000); 
	    break;
	    case 1: 
	    break;
	    case 2: 
	      TIM_Cmd(TIM4, DISABLE);
	      psC_OUT_Func->init(1);
	    break;
  	}
	}
}

void DL212_EasyMode_Config(void){
  unsigned int i,timeout=0;
	char message[100];
	
	if(0 == strncmp("DL212 easy mode run",(const char*)(sUSB_Para.rx_buf),19) ||
		 0 == strncmp("dl212 easy mode run",(const char*)(sUSB_Para.rx_buf),19)){ 
	  DL212_EasyMode = 1;
		vTaskResume(Task2_Handler);
		DL212_EasyMode_Init();
    i=sprintf(message,"DL212简易模式运行中...\r\n\r\n");USB_Send((unsigned char *)message,i); 	 
  }
	else if(0 == strncmp("DL212 easy mode exit",(const char*)(sUSB_Para.rx_buf),20) ||
		      0 == strncmp("dl212 easy mode exit",(const char*)(sUSB_Para.rx_buf),20)){
	  DL212_EasyMode = 0;
		DL212_Value_Display_Ctrl = 0;				
    vTaskSuspend(Task2_Handler);
    i=sprintf(message,"DL212简易模式停止运行...\r\n\r\n");USB_Send((unsigned char *)message,i); 	 						
	}
	else if(0 == strncmp("DL212 easy mode config",(const char*)(sUSB_Para.rx_buf),22) ||
		 0 == strncmp("dl212 easy mode config",(const char*)(sUSB_Para.rx_buf),22)){ 
		DL212_EasyMode = 1; 
		vTaskResume(Task2_Handler);	 
		P_SW_Time=C1_Time=C2_Time=F_Mea_Time=1000;
		P_SW_Option=C1_Option=C2_Option=F_Mea_Option=1;
    P_SW_Dest=0,C1_Dest=1,C2_Dest=2,F_Mea_Dest=3;
		sUSB_Para.rec_len = 0; 
			 
		//step 1.
	  i=sprintf(message,"是否恢复缺省配置[y/n](n):\r\n\r\n");USB_Send((unsigned char *)message,i); 
		while(timeout++ < 0xF0000000){ 
		  CDC_Receive_DATA(); 
	    if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){ 
			  timeout = 0; 
        if(0==strncmp("Y",(const char*)(sUSB_Para.rx_buf),1) || 0==strncmp("y",(const char*)(sUSB_Para.rx_buf),1)){ 
					memset(&sEMData,0,sizeof(sEMData)); 
					sEMData.scan_intvl=5,sEMData.addr_0='A',sEMData.addr_1='A'; 
					sEMData.usart_baudrate=115200,sEMData.usart_mode=0,sEMData.modbus_address=1;
          EEPROM_Write((unsigned char*)&sEMData,EEPROM_BANK_START_ADDR,sizeof(sEMData)); 
					i=sprintf(message,"已恢复缺省配置,DL212简易模式运行中...\r\n\r\n");USB_Send((unsigned char *)message,i);					
				  return; 
			  } 
				else if(0==strncmp("N",(const char*)(sUSB_Para.rx_buf),1) || 0==strncmp("n",(const char*)(sUSB_Para.rx_buf),1)){ 
				  break; 
				} 
				else{ 
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i); 
					i=sprintf(message,"是否恢复缺省配置[y/n](n)\r\n\r\n");USB_Send((unsigned char *)message,i);
		 		} 
				sUSB_Para.rec_len = 0;
		  }
	  }
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 2.
		i=sprintf(message,"\r\n设备标识(%c%c):\r\n\r\n",sEMData.addr_0,sEMData.addr_1);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA(); 
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0; 
				if(0==isalnum(*sUSB_Para.rx_buf) || 0==isalnum(*(sUSB_Para.rx_buf+1)) ||  4!=sUSB_Para.rec_len){
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"\r\n设备标识(%c%c)\r\n\r\n",sEMData.addr_0,sEMData.addr_1);USB_Send((unsigned char *)message,i);
				}
        else{
					 sEMData.addr_0=*(sUSB_Para.rx_buf),sEMData.addr_1=*(sUSB_Para.rx_buf+1);
           EEPROM_Write((unsigned char*)&sEMData.addr_0,((unsigned int)&sEMData.addr_0-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.addr_0)); 
           EEPROM_Write((unsigned char*)&sEMData.addr_1,((unsigned int)&sEMData.addr_1-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.addr_1)); 
  			   i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					 break;
				}	 
				sUSB_Para.rec_len = 0;
		  } 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 3.
		i=sprintf(message,"测量时间间隔(%d)(秒):\r\n\r\n",sEMData.scan_intvl);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(i <= 86400){
					sEMData.scan_intvl = i; 
					EEPROM_Write((unsigned char*)&sEMData.scan_intvl,((unsigned int)&sEMData.scan_intvl-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.scan_intvl)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
					i=sprintf(message,"测量时间间隔(%d)(秒):\r\n\r\n",sEMData.scan_intvl);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){ 
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return; 
		} 
		//step 4.
		i=sprintf(message,"V1端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sEMData.v1_func);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sEMData.v1_func = i;
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					switch(i){ 
						case 1: 
							psSE_FUNC->chan_open(0);
				      TIM10_Init(1000);
						break;
						case 2: 
							TIM_Cmd(TIM10, DISABLE);
						break;
						default:
						break;
					}
          EEPROM_Write((unsigned char*)&sEMData.v1_func,((unsigned int)&sEMData.v1_func-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.v1_func)); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"V1端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sEMData.v1_func);USB_Send((unsigned char *)message,i);
				}
				sUSB_Para.rec_len = 0;
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 5.
		i=sprintf(message,"V2端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sEMData.v2_func);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sEMData.v2_func = i;
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					switch(i){ 
						case 1: 
							psSE_FUNC->chan_open(1);
				      TIM10_Init(1000);
						break;
						case 2: 
							TIM_Cmd(TIM10, DISABLE);
						break;
						default:
						break;
					}
          EEPROM_Write((unsigned char*)&sEMData.v2_func,((unsigned int)&sEMData.v2_func-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.v2_func)); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"V2端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sEMData.v2_func);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 6.
		i=sprintf(message,"V3端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sEMData.v3_func);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sEMData.v3_func = i;
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					switch(i){ 
						case 1: 
							psSE_FUNC->chan_open(2);
				      TIM10_Init(1000);
						break;
						case 2: 
							TIM_Cmd(TIM10, DISABLE);
						break;
						default:
						break;
					}
          EEPROM_Write((unsigned char*)&sEMData.v3_func,((unsigned int)&sEMData.v3_func-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.v3_func)); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"V3端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sEMData.v3_func);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 7.
		i=sprintf(message,"V4端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sEMData.v4_func);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sEMData.v4_func = i;
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					switch(i){ 
						case 1: 
							psSE_FUNC->chan_open(3);
				      TIM10_Init(1000);
						break;
						case 2: 
							TIM_Cmd(TIM10, DISABLE);
						break;
						default:
						break;
					}
          EEPROM_Write((unsigned char*)&sEMData.v4_func,((unsigned int)&sEMData.v4_func-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.v4_func)); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"V4端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sEMData.v4_func);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 8.
		i=sprintf(message,"F1端口设置[0.数字量测量1.禁用](%d):\r\n\r\n",sEMData.f1_func);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i){
					sEMData.f1_func = i;
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					switch(i){
					  case 0: 
				      TIM6_Init(1000);
						break;
						case 1: 
				      TIM_Cmd(TIM6,DISABLE);;
						break; 
					}
          EEPROM_Write((unsigned char*)&sEMData.f1_func,((unsigned int)&sEMData.f1_func-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.f1_func)); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"F1端口设置[0.数字量测量1.禁用](%d):\r\n\r\n",sEMData.f1_func);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 9. 
		i=sprintf(message,"D1端口设置[0.数字量测量1.SDI12测量2.开关输出](%d):\r\n\r\n",sEMData.d1_func);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf); 
				if(0==i || 1==i || 2==i){
					sEMData.d1_func = i;
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					switch(i){
					  case 0: 
				      psC_Pulse_Func->init(0);
						  TIM7_Init(1000); 
						break;
						case 1: 
						break;
						case 2: 
							TIM_Cmd(TIM7, DISABLE);
						  psC_OUT_Func->init(0);
						break;
					} 
          EEPROM_Write((unsigned char*)&sEMData.d1_func,((unsigned int)&sEMData.d1_func-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.d1_func)); 
					break;
				} 
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"D1端口设置[0.数字量测量1.SDI12测量2.开关输出](%d):\r\n\r\n",sEMData.d1_func);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 10.
		i=sprintf(message,"D2端口设置[0.数字量测量1.SDI12测量2.开关输出](%d):\r\n\r\n",sEMData.d2_func);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sEMData.d2_func = i;
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					switch(i){
					  case 0: 
				      psC_Pulse_Func->init(1);
						  TIM4_Init(1000); 
						break;
						case 1: 
						break;
						case 2: 
							TIM_Cmd(TIM4, DISABLE);
						  psC_OUT_Func->init(1);
						break;
					} 
          EEPROM_Write((unsigned char*)&sEMData.d2_func,((unsigned int)&sEMData.d2_func-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.d2_func)); 
					break;
				} 
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"D2端口设置[0.数字量测量1.SDI12测量2.开关输出](%d):\r\n\r\n",sEMData.d2_func);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 11.
		i=sprintf(message,"SW12-1端口设置[0.禁用1.使能](%d):\r\n\r\n",sEMData.sw12_1_func);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i){		
					sEMData.sw12_1_func = i;
					EEPROM_Write((unsigned char*)&sEMData.sw12_1_func,((unsigned int)&sEMData.sw12_1_func-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.sw12_1_func)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"SW12-1端口设置[0.禁用1.使能](%d):\r\n\r\n",sEMData.sw12_1_func);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 12.
		i=sprintf(message,"SW12-2端口设置[0.禁用1.使能](%d):\r\n\r\n",sEMData.sw12_2_func);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i){
					sEMData.sw12_2_func = i;
					EEPROM_Write((unsigned char*)&sEMData.sw12_2_func,((unsigned int)&sEMData.sw12_2_func-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.sw12_2_func)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"SW12-2端口设置[0.禁用1.使能](%d):\r\n\r\n",sEMData.sw12_2_func);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		
		//step 13.
		i=sprintf(message,"\r\n串口波特率(%d):\r\n\r\n",sEMData.usart_baudrate);USB_Send((unsigned char *)message,i);
    while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(1200==i || 2400==i || 4800==i || 9600==i || 14400==i || 19200==i || 38400==i || 57600==i || 115200==i){
					sEMData.usart_baudrate = i;
					USART1_Config(i);
					EEPROM_Write((unsigned char*)&sEMData.usart_baudrate,((unsigned int)&sEMData.usart_baudrate-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.usart_baudrate)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
       		i=sprintf(message,"\r\n串口波特率(%d):\r\n\r\n",sEMData.usart_baudrate);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 14.
		i=sprintf(message,"\r\nmodbus从机地址设置(%d):\r\n\r\n",sEMData.modbus_address);USB_Send((unsigned char *)message,i);
    while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(i>=1 && i<=247){
					sEMData.modbus_address = i; 
					EEPROM_Write((unsigned char*)&sEMData.modbus_address,((unsigned int)&sEMData.modbus_address-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.usart_mode)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
	      	i=sprintf(message,"\r\nmodbus从机地址设置(%d):\r\n\r\n",sEMData.modbus_address);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		}
		//step 15.
		i=sprintf(message,"\r\n串口模式选择[0.ascii自动发送1.modbus模式](%d):\r\n\r\n",sEMData.usart_mode);USB_Send((unsigned char *)message,i);
    while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i){
					sEMData.usart_mode = i;
					EEPROM_Write((unsigned char*)&sEMData.usart_mode,((unsigned int)&sEMData.usart_mode-(unsigned int)&sEMData+EEPROM_BANK_START_ADDR),sizeof(sEMData.usart_mode)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
			  	i=sprintf(message,"配置完成,DL212简易模式运行中...\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"\r\n串口模式选择[0.ascii自动发送1.modbus](%d):\r\n\r\n",sEMData.usart_mode);USB_Send((unsigned char *)message,i);
				} 
				sUSB_Para.rec_len = 0; 
			} 
		} 
		sUSB_Para.rec_len = 0; 
		if(timeout){
		  i=sprintf(message,"配置超时,如需配置请重新进入配置模式\r\n\r\n");USB_Send((unsigned char *)message,i);
			return;
		} 
	} 
	else{
	  return;
	}
} 
