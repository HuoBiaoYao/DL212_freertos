#ifndef  __HW_TIM_H
#define  __HW_TIM_H
 
void TIM2_TI2FP2_Init(void);
void TIM6_Init(unsigned int ms);//basic timer for tim2

void TIM3_ETR_Init(void);
void TIM4_Init(unsigned int ms);//basic timer for tim3

void TIM9_TI2FP2_Init(void);
void TIM7_Init(unsigned int ms);//basic timer for tim9

void TIM5_TI1FP1_Init(void);
void TIM10_Init(unsigned int ms);//basic timer for tim5
  
#endif
