#include "stm32l1xx_rtc.h"
#include "hw_stm32_rtc.h"
 

void HW_STM32_RTC_Init(void){
  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
  PWR_RTCAccessCmd(ENABLE);
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  RCC_LSICmd(ENABLE);//for rtc wakeup
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET){
    
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  RCC_RTCCLKCmd(ENABLE);
  RTC_WaitForSynchro();

  EXTI_ClearITPendingBit(EXTI_Line20);
  EXTI_InitStructure.EXTI_Line = EXTI_Line20;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(2048);//1000ms   32768/16=2048

  RTC_ITConfig(RTC_IT_WUT, ENABLE);
	RTC_WakeUpCmd(DISABLE);
}
 