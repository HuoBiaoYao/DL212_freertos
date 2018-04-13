#include "hw.h"
#include "hal.h"
#include "stm32l1xx_it.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "dl212.h"

/*rtc int*/
void EXTI2_IRQHandler(void){
  if(EXTI_GetITStatus(EXTI_Line2) != RESET){
		RTC_M41T81_IntCount++;
  }
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI15_10_IRQHandler(void){
  if(EXTI_GetITStatus(USB_DETECT_EXTI_LINE) != RESET){
 
  }
	EXTI_ClearITPendingBit(USB_DETECT_EXTI_LINE);
}

/*gprs and zigbee*/
void DMA1_Channel4_IRQHandler(void){
  DMA_ClearFlag(DMA1_FLAG_TC4);
  DMA_Cmd(DMA1_Channel4,DISABLE);
	USART1_DMA_Send_State = 0;
}
/*gprs and zigbee
void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET){
		USART1_DMA_Rec_Cnt = USART1->SR;
    USART1_DMA_Rec_Cnt = USART1->DR; //clear USART_IT_IDLE flag
		DMA_Cmd(DMA1_Channel5,DISABLE);
		USART1_DMA_Rec_Cnt = 64-DMA_GetCurrDataCounter(DMA1_Channel5);
    DMA_SetCurrDataCounter(DMA1_Channel5,64);
    DMA_Cmd(DMA1_Channel5,ENABLE);
	}
	USART_ClearITPendingBit(USART1,USART_IT_IDLE);
}*/
/*c1 sdi12 or rs232 reiceive*/
void USART2_IRQHandler(void){
  if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
		psSDI12_Func->read(0,sSDI12_Para[0].rx_buf,(unsigned char*)&USART2->DR);
		psC_RS232_Func->read(0,sC_RS232_Para[0].rx_buf,(unsigned char*)&USART2->DR);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
} 
/*c2 sdi12 or rs232 reiceive*/
void UART5_IRQHandler(void){
  if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(UART5,USART_IT_RXNE); 
		psSDI12_Func->read(1,sSDI12_Para[1].rx_buf,(unsigned char*)&UART5->DR);
		psC_RS232_Func->read(1,sC_RS232_Para[1].rx_buf,(unsigned char*)&UART5->DR);
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
	}
} 
 
void TIM7_IRQHandler(void){ 
	unsigned int count;
	
  if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET){
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		count = TIM9->CNT;
	  TIM9->CNT = 0; 
		//TIM_Cmd(TIM7,DISABLE);
		switch(C1_Option){
			case 0:
			  Var[C1_Dest] = (double)(C1_Time)/(double)(count);
		  break;
		  case 1:
			  Var[C1_Dest] = (double)(count*1000)/(double)(C1_Time);
		  break;
		  case 2:
			  Var[C1_Dest] = count;
		  break;
		  default:
		  break;
	  }			
	}
} 
void TIM4_IRQHandler(void){ 
  volatile unsigned int count=0;
	
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		count = TIM3->CNT;
	  TIM3->CNT = 0; 
	  //TIM_Cmd(TIM4,DISABLE);
	  switch(C2_Option){
			case 0:
			  Var[C2_Dest] = (double)(C2_Time)/(double)(count);
		  break;
		  case 1:
			  Var[C2_Dest] = (double)(count*1000)/(double)(C2_Time);
		  break;
		  case 2:
			  Var[C2_Dest] = count;
		  break;
		  default:
		  break;
	  }			 
	}
}
void TIM6_IRQHandler(void){ 
  volatile unsigned int count=0;
	
  if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update); 
    count=TIM2->CNT;
    TIM2->CNT = 0;
    //TIM_Cmd(TIM6,DISABLE);
    switch(P_SW_Option){
      case 0:
        Var[P_SW_Dest] = (double)(P_SW_Time)/(double)(count);
		  break;
		  case 1:
			  Var[P_SW_Dest] = (double)(count*1000)/(double)(P_SW_Time);
		  break;
		  case 2:
		   Var[P_SW_Dest] = count;
		  break;
		  default:
		  break;
	  }
	}
} 
void TIM10_IRQHandler(void){ 
  unsigned int count;
	
  if(TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET){ 
    TIM_ClearITPendingBit(TIM10, TIM_IT_Update); 
		count = TIM5->CNT; 
	  TIM5->CNT = 0; 
		//TIM_Cmd(TIM10,DISABLE); 
		switch(F_Mea_Option){
			case 0:
			  Var[F_Mea_Dest] = (double)(F_Mea_Time)/(double)(count);
		  break;
		  case 1:
			  Var[F_Mea_Dest] = (double)(count*1000)/(double)(F_Mea_Time);;
		  break;
		  case 2:
			  Var[F_Mea_Dest] = count;
		  break;
		  default:
		  break;
	  }	 
	}
}
 
/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

 
/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}
 

/*******************************************************************************
* Function Name  : USB_IRQHandler
* Description    : This function handles USB Low Priority interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)|| defined (STM32F37X)
void USB_LP_IRQHandler(void)
#else
void USB_LP_CAN1_RX0_IRQHandler(void)
#endif
{
  USB_Istr();
}

 
/*******************************************************************************
* Function Name  : USB_FS_WKUP_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
void USB_FS_WKUP_IRQHandler(void)
#else
void USBWakeUp_IRQHandler(void)
#endif
{
  EXTI_ClearITPendingBit(EXTI_Line18);
}

/******************************************************************************/
/*                 STM32 Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32xxx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/

/*
float TIM4Freq[4]={0};
unsigned int TIM4_Dif[4]={0},IC4_Value_New[4]={0},IC4_Value[4]={0},TIM4_CaptureNum[4]={0},TIM4_Cycle[4]={0};
void TIM4_IRQHandler(void){
	unsigned char p=0xFF;
	
  if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET){
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		p = 0;
	}
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET){
	  TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		p = 1;
	}
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET){
	  TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		p = 2;
	}
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET){
	  TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		p = 3;
	}
	if(p <= 3){
	  if(0 == TIM4_CaptureNum[p]){ 
			IC4_Value[p] = *(volatile unsigned int*)(0x40000834+p*4);
		  //IC4_Value[p] = TIM_GetCapture2(TIM4);
			TIM4_CaptureNum[p]++;
		}
    else if(1 == TIM4_CaptureNum[p]){
			IC4_Value_New[p] = *(volatile unsigned int*)(0x40000834+p*4);
			//IC4_Value[p] = TIM_GetCapture2(TIM4);
			if(IC4_Value_New[p] > IC4_Value[p]){ 
			  TIM4_Dif[p] += IC4_Value_New[p]-IC4_Value[p]-1;  
			}
			else if(IC4_Value_New[p] < IC4_Value[p]){
			  TIM4_Dif[p] += 0xFFFF+IC4_Value_New[p]-IC4_Value[p]-1;  
			}
			else{
			  TIM4_Dif[p] = 0;
			}
			TIM4_CaptureNum[p] = 0;
			*(volatile unsigned int*)(0x40000834+p*4) = 0;
			//TIM_SetCompare2(TIM4,0);
			TIM4_Cycle[p]++;
			if(10 == TIM4_Cycle[p]){
			  if(0 != TIM4_Dif[p]){
			    TIM4Freq[p] = (float)SystemCoreClock*TIM4_Cycle[p]/(TIM4_Dif[p]);
			  }
			  else{
			  	TIM4Freq[p] = 0;
			  }
				TIM4_Cycle[p] = TIM4_Dif[p] = 0;
			}
		} 
  } 
}*/
 
 
/*
double TIM5Freq=0; 
unsigned int TIM5_Dif=0,IC5_Value_New=0,IC5_Value=0,TIM5_Cycle=0; 
void TIM5_IRQHandler(void){ 
  if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET){ 
		if(0 == TIM5_Cycle){ 
		  IC5_Value = TIM5->CCR1; 
			TIM5_Cycle++; 
		} 
    else if(TIM5_Cycle){ 
			IC5_Value_New = TIM5->CCR1; 
	    TIM5_Dif += IC5_Value_New-IC5_Value; 
			IC5_Value = IC5_Value_New; 
			TIM5_Cycle++; 
			if((Period_Cycles+1) == TIM5_Cycle){ 
				TIM_SetCompare1(TIM5,0); 
				TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE); 
        TIM5Freq = (double)SystemCoreClock*(TIM5_Cycle-1)/(double)(TIM5_Dif);  
			} 
		} 
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); 
  } 
} 
 */
 