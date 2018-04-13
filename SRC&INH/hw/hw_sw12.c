#include "hw_sw12.h"
#include "stm32l1xx_gpio.h"

void SW12_Port_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(SW12_1_CLK|SW12_2_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Pin = SW12_1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(SW12_1_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = SW12_2_PIN; 
  GPIO_Init(SW12_2_PORT, &GPIO_InitStructure); 
}

void SW12(unsigned char port,unsigned char state){
  switch(port){
    case 0:
      if(state == 1){
			  SW12_1_ON();
			}
			else{
			  SW12_1_OFF();
			}
  	break;
	  case 1:
      if(state == 1){
			  SW12_2_ON();
			}
			else{
			  SW12_2_OFF();
			}
  	break;
		default:
		break;
	}
}
