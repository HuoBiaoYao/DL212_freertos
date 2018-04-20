#include "hw_tim.h"
#include "stm32l1xx_tim.h"
 
/*TIM2 TI2FP2 
  PS1---PA1(TIM2_CH2)
*/
void TIM2_TI2FP2_Init(void){  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
 
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // Time base configuration
	TIM_TIxExternalClockConfig(TIM2,TIM_TIxExternalCLK1Source_TI2,TIM_ICPolarity_Rising,0);
	TIM_SetCounter(TIM2, 0);
  TIM_Cmd(TIM2, ENABLE);
}

/*TIM3 ETR
  C2---PD2(TIM3_ETR)
*/
void TIM3_ETR_Init(void){  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOD, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_TIM3);
 
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); // Time base configuration
  TIM_ETRClockMode2Config(TIM3, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM3, 0);
  TIM_Cmd(TIM3, ENABLE); 
}

/*TIM9 TI2FP2
  C1---PA3(TIM9_CH2)
*/
void TIM9_TI2FP2_Init(void){  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE); 
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);
 
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure); // Time base configuration
	TIM_TIxExternalClockConfig(TIM9,TIM_TIxExternalCLK1Source_TI2,TIM_ICPolarity_Rising,0);
	TIM_SetCounter(TIM9, 0);
  TIM_Cmd(TIM9, ENABLE);
}

/*TIM5 TI1FP1
  F_Mea---PA0(TIM5_CH1)
*/
void TIM5_TI1FP1_Init(void){  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  GPIO_InitTypeDef GPIO_InitStructure;  
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA, ENABLE); 
	
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; 
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); 
  
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); // Time base configuration
	TIM_TIxExternalClockConfig(TIM5,TIM_TIxExternalCLK1Source_TI1,TIM_ICPolarity_Rising,0);
	TIM_SetCounter(TIM5, 0); 
  TIM_Cmd(TIM5, ENABLE); 
}

 
/*TIM6 for PSW-----PA1(TIM2_CH2)*/
void TIM6_Init(unsigned int ms){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  NVIC_InitTypeDef NVIC_InitStructure; 
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  TIM_TimeBaseStructure.TIM_Period = ms*10-1;
  TIM_TimeBaseStructure.TIM_Prescaler = 3200-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	 
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);//如果不清除标志，下面配置中断后，会立刻进入一次中断
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(TIM6, ENABLE);
}

/*TIM4 for C2------PD2(TIM3_ETR)*/
void TIM4_Init(unsigned int ms){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  NVIC_InitTypeDef NVIC_InitStructure; 
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  TIM_TimeBaseStructure.TIM_Period = ms*10-1;
  TIM_TimeBaseStructure.TIM_Prescaler = 3200-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//如果不清除标志，下面配置中断后，会立刻进入一次中断	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(TIM4, ENABLE);
}

/*TIM7 for C1------PA3(TIM9_CH2)*/
void TIM7_Init(unsigned int ms){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  NVIC_InitTypeDef NVIC_InitStructure; 
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

  TIM_TimeBaseStructure.TIM_Period = ms*10-1;
  TIM_TimeBaseStructure.TIM_Prescaler = 3200-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	 
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);//如果不清除标志，下面配置中断后，会立刻进入一次中断	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(TIM7, ENABLE);
}

/*TIM10 for F_Mea---PA0(TIM5_CH1)*/
void TIM10_Init(unsigned int ms){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  NVIC_InitTypeDef NVIC_InitStructure; 
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
  
  TIM_TimeBaseStructure.TIM_Period = ms*10-1;
  TIM_TimeBaseStructure.TIM_Prescaler = 3200-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);
	 
	TIM_ClearFlag(TIM10, TIM_FLAG_Update);//如果不清除标志，下面配置中断后，会立刻进入一次中断
	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(TIM10, ENABLE);
}

/*TIM2 TI1F_ED,上升沿和下降沿都检测,频率要除以2
void TIM2_TI1F_ED_Init(void){  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
 
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // Time base configuration
	TIM_TIxExternalClockConfig(TIM2,TIM_TIxExternalCLK1Source_TI1ED,TIM_ICPolarity_Rising,0);
	TIM_SetCounter(TIM2, 0);
  TIM_Cmd(TIM2, ENABLE);
}*/
/*输入捕获
void TIM5_InputCapture_Init(void){ 
	TIM_ICInitTypeDef TIM_ICInitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
 
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
   
  TIM_Cmd(TIM5, ENABLE); 
  TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE); 
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}
*/
 
/*输入捕获
void TIM4_InputCaptureRouting_Init(unsigned char port){ 
	TIM_ICInitTypeDef TIM_ICInitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 | RCC_APB1Periph_COMP | RCC_APB1Periph_PWR, ENABLE);
 
	switch (port){
	  case 1:
			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
      GPIO_Init(GPIOC, &GPIO_InitStructure); 
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_RI); 
      SYSCFG_RITIMSelect(TIM_Select_TIM4); 
      SYSCFG_RITIMInputCaptureConfig(RI_InputCapture_IC1, RI_InputCaptureRouting_4); 
      TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
      TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
      TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
      TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
      TIM_ICInitStructure.TIM_ICFilter = 0x0;
      TIM_ICInit(TIM4, &TIM_ICInitStructure);
		  TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
		break;
		case 2:
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
      GPIO_Init(GPIOC, &GPIO_InitStructure); 
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_RI); 
      SYSCFG_RITIMSelect(TIM_Select_TIM4); 
      SYSCFG_RITIMInputCaptureConfig(RI_InputCapture_IC2, RI_InputCaptureRouting_4); 
      TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
      TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
      TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
      TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
      TIM_ICInitStructure.TIM_ICFilter = 0x0;
      TIM_ICInit(TIM4, &TIM_ICInitStructure);
		  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
		break;
		case 3:
			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
      GPIO_Init(GPIOC, &GPIO_InitStructure); 
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_RI); 
      SYSCFG_RITIMSelect(TIM_Select_TIM4); 
      SYSCFG_RITIMInputCaptureConfig(RI_InputCapture_IC3, RI_InputCaptureRouting_4); 
      TIM_ICInitStructure.TIM_Channel     = TIM_Channel_3;
      TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
      TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
      TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
      TIM_ICInitStructure.TIM_ICFilter = 0x0;
      TIM_ICInit(TIM4, &TIM_ICInitStructure);
		  TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
		break;
		case 4:
			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
      GPIO_Init(GPIOC, &GPIO_InitStructure); 
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_RI); 
      SYSCFG_RITIMSelect(TIM_Select_TIM4); 
      SYSCFG_RITIMInputCaptureConfig(RI_InputCapture_IC4, RI_InputCaptureRouting_4); 
      TIM_ICInitStructure.TIM_Channel     = TIM_Channel_4;
      TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
      TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
      TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
      TIM_ICInitStructure.TIM_ICFilter = 0x0;
      TIM_ICInit(TIM4, &TIM_ICInitStructure);
		  TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
		break;
		default: 
		break;
	} 
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
  TIM_Cmd(TIM4, ENABLE); 
  
}*/
 