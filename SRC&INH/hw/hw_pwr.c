#include "hw_pwr.h"
#include "stm32l1xx_gpio.h"

void Power_SW_Port_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(POWER_V0_CLK|POWER_V1_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = POWER_V0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(POWER_V0_PORT, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = POWER_V1_PIN; 
  GPIO_Init(POWER_V1_PORT, &GPIO_InitStructure); 
}

void Power_SW(unsigned char powertype,unsigned char state){
  switch(powertype){
    case 0:
      if(state == 1){
			  POWER_V0_ON();
			}
			else{
			  POWER_V0_OFF();
			}
  	break;
	  case 1:
      if(state == 1){
			  POWER_V1_ON();
			}
			else{
			  POWER_V1_OFF();
			}
  	break;
		default:
		break;
	}
}
 
	
 