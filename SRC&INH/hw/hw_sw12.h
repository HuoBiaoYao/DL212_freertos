#ifndef __HW_SW12_H
#define __HW_SW12_H

#include "main.h"
  
#define SW12_1_PIN    GPIO_Pin_13
#define SW12_1_PORT   GPIOC
#define SW12_1_CLK    RCC_AHBPeriph_GPIOC

#define SW12_2_PIN    GPIO_Pin_14
#define SW12_2_PORT   GPIOC
#define SW12_2_CLK    RCC_AHBPeriph_GPIOC
 
#define SW12_1_ON()   GPIO_SetBits(SW12_1_PORT,SW12_1_PIN)  
#define SW12_1_OFF()  GPIO_ResetBits(SW12_1_PORT,SW12_1_PIN)
#define SW12_2_ON()   GPIO_SetBits(SW12_2_PORT,SW12_2_PIN)
#define SW12_2_OFF()  GPIO_ResetBits(SW12_2_PORT,SW12_2_PIN)


void SW12_Port_Init(void);
void SW12(unsigned char port,unsigned char state);




#endif
