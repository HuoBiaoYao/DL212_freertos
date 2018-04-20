#include "hw_se_port.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_adc.h"
#include "delay.h"

 

void SE_ADC_Init(void){
  ADC_InitTypeDef  ADC_InitStructure;
 
	RCC_HSICmd(ENABLE);
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
  ADC_DeInit(ADC1);
  ADC_BankSelection(ADC1, ADC_Bank_A); 
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 
  ADC_InitStructure.ADC_ScanConvMode =DISABLE; 
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //ADC_InitStructure.ADC_ExternalTrigConv = 0x0C000000;                            
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
 
	ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze); 
  ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);  
  ADC_Cmd(ADC1, ENABLE); 
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET){}
}  

float SE_AdcValue_Read(unsigned char chan){
	unsigned int adcv; 
  float vref;
 
	RCC_HSICmd(ENABLE);
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE); 
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET){}
	SE_Chan_Close();
	switch (chan){
		case 0:
			ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_384Cycles); 
		break;
    case 1:
			ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_16Cycles); 
		break;
    case 2:
			ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_16Cycles); 
		break	;	
    case 3:
			ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_16Cycles); 
		break; 
		default:
		break;
	}  
  ADC_SoftwareStartConv(ADC1);  
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET){}
  adcv = ADC_GetConversionValue(ADC1); 
  vref = (float)adcv*3300/4096;   
  ADC_Cmd(ADC1,DISABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
	RCC_HSICmd(DISABLE);	
		
  return vref;
}

float Vref_Read(void){
  unsigned int adcv;  
	float vref;
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,1,ADC_SampleTime_16Cycles); 
	ADC_SoftwareStartConv(ADC1);
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); 
  adcv = ADC_GetConversionValue(ADC1); 
	vref = (float)adcv*3300/4096;
	
	return vref;
}

float Bat_Read(void){
	unsigned int adcv;  
	float bat;
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_16Cycles); 
	ADC_SoftwareStartConv(ADC1); 
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); 
  adcv = ADC_GetConversionValue(ADC1); 
	bat = (float)adcv*3300/4096; 
	bat = 3.985*bat;
	
	return bat;
}

void SE_Chan_Open(unsigned char chan){
  switch(chan){ 
	  case 1: 
		  GPIOB->BSRRH = GPIO_Pin_8;
			GPIOB->BSRRH = GPIO_Pin_9;
		break; 
		case 2: 
			GPIOB->BSRRH = GPIO_Pin_8;
      GPIOB->BSRRL = GPIO_Pin_9;
		break;
		case 3: 
		  GPIOB->BSRRL = GPIO_Pin_8;
      GPIOB->BSRRH = GPIO_Pin_9;
		break;
		case 4:
			GPIOB->BSRRL = GPIO_Pin_8;
      GPIOB->BSRRL = GPIO_Pin_9;
		break;
		default:
		break;
	}
	GPIOB->BSRRH = GPIO_Pin_5;
}


void SE_Chan_Close(void){
  GPIOB->BSRRL = GPIO_Pin_5;
}

 
void SE_OUT_Init(unsigned char chan){
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);  
  switch(chan){
		case 0:
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT; 
	    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
		break;
		case 1:
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT; 
	    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
		break;
		case 2:
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT; 
	    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
		break;
		case 3:
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT; 
	    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
		break;
		default:
		break; 
	}
}
	
void SE_OUT_Ctrl(unsigned char chan,unsigned char state){
  switch(chan){
	  case 0:
		  if(state){
			  GPIOC->BSRRL = GPIO_Pin_0;
			}
			else{
			  GPIOC->BSRRH = GPIO_Pin_0;
			}
		break;
		case 1:
		  if(state){
			  GPIOC->BSRRL = GPIO_Pin_1;
			}
			else{
			  GPIOC->BSRRH = GPIO_Pin_1;
			}
		break;
    case 2:
		  if(state){
			  GPIOC->BSRRL = GPIO_Pin_2;
			}
			else{
			  GPIOC->BSRRH = GPIO_Pin_2;
			}
		break;
    case 3:
		  if(state){
			  GPIOC->BSRRL = GPIO_Pin_3;
			}
			else{
			  GPIOC->BSRRH = GPIO_Pin_3;
			}
		break;			
	}
}
