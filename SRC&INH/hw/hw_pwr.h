#ifndef __HW_PWR_H
#define __HW_PWR_H
#include "main.h"
  
#define POWER_V0_PIN   GPIO_Pin_4
#define POWER_V0_PORT  GPIOA
#define POWER_V0_CLK   RCC_AHBPeriph_GPIOA

#define POWER_V1_PIN   GPIO_Pin_8 
#define POWER_V1_PORT  GPIOA 
#define POWER_V1_CLK   RCC_AHBPeriph_GPIOA  
 
#define POWER_V0_ON()  GPIO_ResetBits(POWER_V0_PORT, POWER_V0_PIN)
#define POWER_V0_OFF() GPIO_SetBits  (POWER_V0_PORT, POWER_V0_PIN)
#define POWER_V1_ON()  GPIO_ResetBits(POWER_V1_PORT, POWER_V1_PIN)
#define POWER_V1_OFF() GPIO_SetBits  (POWER_V1_PORT, POWER_V1_PIN)


void Power_SW_Port_Init(void);
void Power_SW(unsigned char powertype,unsigned char state);

#endif
