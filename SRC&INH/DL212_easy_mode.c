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

struct CONFIG sDL212_Config;
float Value[7];
unsigned char DL212_EasyMode=1;
unsigned char DL212_Value_Display_Ctrl=0;
unsigned int LastScanIntCount=0;

void DL212_EasyMode_Scan(void){
	unsigned int i=0;
	char message[30];
	
	if(sDL212_Config.scan && RTC_M41T81_IntCount!=LastScanIntCount){ 
	  if(0 == RTC_M41T81_IntCount%sDL212_Config.scan){ 
			LastScanIntCount = RTC_M41T81_IntCount; 
	    switch(sDL212_Config.port[0]){ 
			  case 0:
					Value[0] = psSE_FUNC->read(0);
					if(0 == sDL212_Config.mode){
			      printf("%c%c,v1,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[0]); 
					}
				break;
				case 1:
					Value[0] = Var[F_Mea_Dest];
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v1,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[0]);   
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
					  printf("%c%c,v2,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[1]); 
					}
				break;
				case 1:
					Value[1] = Var[F_Mea_Dest];
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v2,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[1]); 
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
					  printf("%c%c,v3,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[2]);
					}
				break;
				case 1:
					Value[2] = Var[F_Mea_Dest];
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v3,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[2]);
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
					  printf("%c%c,v4,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[3]);
					}
				break;
				case 1:
					Value[3] = Var[F_Mea_Dest];
					if(0 == sDL212_Config.mode){
					  printf("%c%c,v4,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[3]);
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
				  printf("%c%c,f1,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[4]);
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
				    printf("%c%c,d1,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[5]);
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
				    printf("%c%c,d2,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[6]);
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
			/*if(0 == sDL212_Config.mode){
		    printf("%c%c,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1],Value[0],Value[1],Value[2],Value[3],Value[4],Value[5],Value[6]);
			}*/
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
  EEPROM_Read((unsigned char*)&sDL212_Config,EEPROM_BANK_START_ADDR,sizeof(sDL212_Config));
	if(1 == DL212_EasyMode){ 
		P_SW_Time  =C1_Time  =C2_Time  =F_Mea_Time  =1000;
		P_SW_Option=C1_Option=C2_Option=F_Mea_Option=1;
	  P_SW_Dest=0,C1_Dest=1,C2_Dest=2,F_Mea_Dest=3;
		
		if(sDL212_Config.port[7]){
		  psSW12_Func->sw(0,1);
		}
		else{
		  psSW12_Func->sw(0,0);   
		}
		if(sDL212_Config.port[8]){
		  psSW12_Func->sw(1,1);
		}
		else{
		  psSW12_Func->sw(1,0);   
		}
		switch(sDL212_Config.port[0]){ 
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
	  switch(sDL212_Config.port[1]){ 
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
	  switch(sDL212_Config.port[2]){ 
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
	  switch(sDL212_Config.port[3]){ 
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
	  switch(sDL212_Config.port[4]){
	    case 0: 
	      TIM6_Init(1000);
	    break;
	    case 1: 
	      TIM_Cmd(TIM6,DISABLE);;
	    break;  
	  }
	  switch(sDL212_Config.port[5]){
	    case 0: 
	      psC_Pulse_Func->init(0);
	      TIM7_Init(1000); 
	    break;
	    case 1: 
				psSDI12_Func->init(0);
	    break;
	    case 2: 
	      TIM_Cmd(TIM7, DISABLE);
	      psC_OUT_Func->init(0);
	    break;
	  }
	  switch(sDL212_Config.port[6]){
	    case 0: 
	      psC_Pulse_Func->init(1);
	      TIM4_Init(1000); 
	    break;
	    case 1: 
				psSDI12_Func->init(1);
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
					memset(&sDL212_Config,0,sizeof(sDL212_Config)); 
					sDL212_Config.scan=5,sDL212_Config.mark[0]='A',sDL212_Config.mark[1]='A'; 
					sDL212_Config.baudrate=115200,sDL212_Config.mode=0,sDL212_Config.modbus_addr=1;
          EEPROM_Write((unsigned char*)&sDL212_Config,EEPROM_BANK_START_ADDR,sizeof(sDL212_Config)); 
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
		i=sprintf(message,"\r\n设备标识(%c%c):\r\n\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA(); 
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0; 
				if(0==isalnum(*sUSB_Para.rx_buf) || 0==isalnum(*(sUSB_Para.rx_buf+1)) ||  4!=sUSB_Para.rec_len){
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"\r\n设备标识(%c%c)\r\n\r\n",sDL212_Config.mark[0],sDL212_Config.mark[1]);USB_Send((unsigned char *)message,i);
				}
        else{
					 sDL212_Config.mark[0]=*(sUSB_Para.rx_buf),sDL212_Config.mark[1]=*(sUSB_Para.rx_buf+1);
           EEPROM_Write((unsigned char*)&sDL212_Config.mark[0],((unsigned int)&sDL212_Config.mark[0]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.mark[0])); 
           EEPROM_Write((unsigned char*)&sDL212_Config.mark[1],((unsigned int)&sDL212_Config.mark[1]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.mark[1])); 
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
		i=sprintf(message,"测量时间间隔(%d)(秒):\r\n\r\n",sDL212_Config.scan);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(i <= 86400){
					sDL212_Config.scan = i; 
					EEPROM_Write((unsigned char*)&sDL212_Config.scan,((unsigned int)&sDL212_Config.scan-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.scan)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
					i=sprintf(message,"测量时间间隔(%d)(秒):\r\n\r\n",sDL212_Config.scan);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"V1端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[0]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sDL212_Config.port[0] = i;
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
          EEPROM_Write((unsigned char*)&sDL212_Config.port[0],((unsigned int)&sDL212_Config.port[0]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.port[0])); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"V1端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[0]);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"V2端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[1]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sDL212_Config.port[1] = i;
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
          EEPROM_Write((unsigned char*)&sDL212_Config.port[1],((unsigned int)&sDL212_Config.port[1]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.port[1])); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"V2端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[1]);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"V3端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[2]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sDL212_Config.port[2] = i;
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
          EEPROM_Write((unsigned char*)&sDL212_Config.port[2],((unsigned int)&sDL212_Config.port[2]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.port[2])); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"V3端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[2]);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"V4端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[3]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sDL212_Config.port[3] = i;
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
          EEPROM_Write((unsigned char*)&sDL212_Config.port[3],((unsigned int)&sDL212_Config.port[3]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.port[3])); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"V4端口功能选择[0.模拟量测量1.数字量测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[3]);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"F1端口设置[0.数字量测量1.禁用](%d):\r\n\r\n",sDL212_Config.port[4]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i){
					sDL212_Config.port[4] = i;
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					switch(i){
					  case 0: 
				      TIM6_Init(1000);
						break;
						case 1: 
				      TIM_Cmd(TIM6,DISABLE);;
						break; 
					}
          EEPROM_Write((unsigned char*)&sDL212_Config.port[4],((unsigned int)&sDL212_Config.port[4]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.port[4])); 
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"F1端口设置[0.数字量测量1.禁用](%d):\r\n\r\n",sDL212_Config.port[4]);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"D1端口设置[0.数字量测量1.SDI12测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[5]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf); 
				if(0==i || 1==i || 2==i){
					sDL212_Config.port[5] = i;
				  USB_Send((unsigned char *)message,sprintf(message,"ok\r\n\r\n"));
					sUSB_Para.rec_len = 0;
					switch(i){
					  case 0: 
				      psC_Pulse_Func->init(0);
						  TIM7_Init(1000); 
						break;
						case 1: 
							psSDI12_Func->init(0); 
						  //i=sprintf(message,"D1端口SDI-12命令设置,以逗号为分隔符(%s):\r\n\r\n",&sDL212_Config.sdi12[0][0]);USB_Send((unsigned char *)message,i);
              if(sDL212_Config.sdi12[0][0] != 0){           
  						  i=sprintf(message,"D1端口SDI-12命令设置(%s",&sDL212_Config.sdi12[0][0]);
						    USB_Send((unsigned char *)message,i-2);
                i=sprintf(message,"):\r\n\r\n");USB_Send((unsigned char *)message,i); 
							}					 
 							else{
							  i=sprintf(message,"D1端口SDI-12命令设置():\r\n\r\n");USB_Send((unsigned char *)message,i); 
							}
						  while('\r'!=*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) || '\n'!=*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
                CDC_Receive_DATA();  
							}
							strcpy(&sDL212_Config.sdi12[0][0],(const char*)sUSB_Para.rx_buf);
							sDL212_Config.sdi12[0][sUSB_Para.rec_len] = 0;
              i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);  							
						break;
						case 2: 
							TIM_Cmd(TIM7, DISABLE);
						  psC_OUT_Func->init(0);
						break;
					} 
          EEPROM_Write((unsigned char*)&sDL212_Config.port[5],((unsigned int)&sDL212_Config.port[5]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.port[5])); 
          EEPROM_Write((unsigned char*)&sDL212_Config.sdi12[0][0],((unsigned int)&sDL212_Config.sdi12[0][0]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),440); 				
					break;
				} 
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"D1端口设置[0.数字量测量1.SDI12测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[5]);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"D2端口设置[0.数字量测量1.SDI12测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[6]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i || 2==i){
					sDL212_Config.port[6] = i;
				  USB_Send((unsigned char *)message,sprintf(message,"ok\r\n\r\n"));
					sUSB_Para.rec_len = 0;
					switch(i){
					  case 0: 
				      psC_Pulse_Func->init(1);
						  TIM4_Init(1000); 
						break;
						case 1: 
							psSDI12_Func->init(1);
						  //i=sprintf(message,"D2端口SDI-12命令设置,以逗号为分隔符(%s):\r\n\r\n",&sDL212_Config.sdi12[1][0]);USB_Send((unsigned char *)message,i);						
              if(sDL212_Config.sdi12[1][0] != 0){
							  i=sprintf(message,"D2端口SDI-12命令设置(%s",&sDL212_Config.sdi12[1][0]);
						    USB_Send((unsigned char *)message,i-2);
                i=sprintf(message,"):\r\n\r\n");USB_Send((unsigned char *)message,i); 
							}
	  					else{
							  i=sprintf(message,"D2端口SDI-12命令设置	():\r\n\r\n");USB_Send((unsigned char *)message,i); 
							}
  						while('\r'!=*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) || '\n'!=*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
                CDC_Receive_DATA();  
							}
							strcpy(&sDL212_Config.sdi12[1][0],(const char*)sUSB_Para.rx_buf);
							sDL212_Config.sdi12[1][sUSB_Para.rec_len] = 0;
              i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);	
						break;
						case 2: 
							TIM_Cmd(TIM4, DISABLE);
						  psC_OUT_Func->init(1);
						break;
					} 
          EEPROM_Write((unsigned char*)&sDL212_Config.port[6],((unsigned int)&sDL212_Config.port[6]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.port[6])); 
          EEPROM_Write((unsigned char*)&sDL212_Config.sdi12[1][0],((unsigned int)&sDL212_Config.sdi12[1][0]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),440); 
					break;
				} 
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"D2端口设置[0.数字量测量1.SDI12测量2.开关输出](%d):\r\n\r\n",sDL212_Config.port[6]);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"SW12-1端口设置[0.禁用1.使能](%d):\r\n\r\n",sDL212_Config.port[7]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i){		
					sDL212_Config.port[7] = i;
					EEPROM_Write((unsigned char*)&sDL212_Config.port[7],((unsigned int)&sDL212_Config.port[7]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.port[7])); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"SW12-1端口设置[0.禁用1.使能](%d):\r\n\r\n",sDL212_Config.port[7]);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"SW12-2端口设置[0.禁用1.使能](%d):\r\n\r\n",sDL212_Config.port[8]);USB_Send((unsigned char *)message,i);
		while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i){
					sDL212_Config.port[8] = i;
					EEPROM_Write((unsigned char*)&sDL212_Config.port[8],((unsigned int)&sDL212_Config.port[8]-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.port[8])); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"SW12-2端口设置[0.禁用1.使能](%d):\r\n\r\n",sDL212_Config.port[8]);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"\r\n串口波特率(%d):\r\n\r\n",sDL212_Config.baudrate);USB_Send((unsigned char *)message,i);
    while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(1200==i || 2400==i || 4800==i || 9600==i || 14400==i || 19200==i || 38400==i || 57600==i || 115200==i){
					sDL212_Config.baudrate = i;
					USART1_Config(i);
					EEPROM_Write((unsigned char*)&sDL212_Config.baudrate,((unsigned int)&sDL212_Config.baudrate-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.baudrate)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
       		i=sprintf(message,"\r\n串口波特率(%d):\r\n\r\n",sDL212_Config.baudrate);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"\r\nmodbus从机地址设置(%d):\r\n\r\n",sDL212_Config.modbus_addr);USB_Send((unsigned char *)message,i);
    while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(i>=1 && i<=247){
					sDL212_Config.modbus_addr = i; 
					EEPROM_Write((unsigned char*)&sDL212_Config.modbus_addr,((unsigned int)&sDL212_Config.modbus_addr-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.mode)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
	      	i=sprintf(message,"\r\nmodbus从机地址设置(%d):\r\n\r\n",sDL212_Config.modbus_addr);USB_Send((unsigned char *)message,i);
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
		i=sprintf(message,"\r\n串口模式选择[0.ascii自动发送1.modbus模式](%d):\r\n\r\n",sDL212_Config.mode);USB_Send((unsigned char *)message,i);
    while(timeout++ < 0xF0000000){
		  CDC_Receive_DATA();
		  if('\r'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-2) && '\n'==*(sUSB_Para.rx_buf+sUSB_Para.rec_len-1)){
				timeout = 0;
			  i = atoi((const char*)sUSB_Para.rx_buf);
				if(0==i || 1==i){
					sDL212_Config.mode = i;
					EEPROM_Write((unsigned char*)&sDL212_Config.mode,((unsigned int)&sDL212_Config.mode-(unsigned int)&sDL212_Config+EEPROM_BANK_START_ADDR),sizeof(sDL212_Config.mode)); 
				  i=sprintf(message,"ok\r\n\r\n");USB_Send((unsigned char *)message,i);
			  	i=sprintf(message,"配置完成,DL212简易模式运行中...\r\n\r\n");USB_Send((unsigned char *)message,i);
					break;
				}
				else{
				  i=sprintf(message,"输入有误,请重新输入\r\n\r\n");USB_Send((unsigned char *)message,i);
		      i=sprintf(message,"\r\n串口模式选择[0.ascii自动发送1.modbus](%d):\r\n\r\n",sDL212_Config.mode);USB_Send((unsigned char *)message,i);
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

 
unsigned char LRC( unsigned char *buf,unsigned short int len){
  unsigned char lrc=0;

  while(len--){
    lrc += *buf++;
  }  
  lrc = (unsigned char)(-((char)lrc));
	
  return lrc;
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



