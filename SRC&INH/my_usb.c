#include "my_usb.h" 
#include "hw_usb.h"
#include "delay.h" 

struct _USB_PARA sUSB_Para; 

void My_USB_Init(void){
  sUSB_Para.packet_sent = 1; 
  Set_System();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();
}
 
unsigned char USB_Send(unsigned char *buf,int len){
  int i,pack_num;
 
  if(1 == sUSB_Para.packet_sent){
    pack_num = len/63+1;
    for(i=0;i<pack_num-1;i++){
      CDC_Send_DATA(buf+i*63,63);
			delay_ms(1);
    }
    CDC_Send_DATA(buf+i*63,len-i*63);
		delay_ms(1);
	}
	
	return 0;
}
 
