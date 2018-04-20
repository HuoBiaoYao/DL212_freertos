/* Includes ------------------------------------------------------------------*/

#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_usb.h"
#include "usb_pwr.h"
#include "my_usb.h"

ErrorStatus HSEStartUpStatus;
 
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
/* Extern variables ----------------------------------------------------------*/

extern LINE_CODING linecoding;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  RCC_AHBPeriphClockCmd(USB_DETECT_CLK,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = USB_DETECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(USB_DETECT_PORT, &GPIO_InitStructure); 
  
	SYSCFG_EXTILineConfig(USB_DETECT_PORTSOURCE,USB_DETECT_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = USB_DETECT_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_DETECT_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
   /* Configure the EXTI line 18 connected internally to the USB IP */
  EXTI_ClearITPendingBit(EXTI_Line18);
  EXTI_InitStructure.EXTI_Line = EXTI_Line18;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void){
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD) || defined(STM32L1XX_MD_PLUS) 
  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#else 
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#endif /* STM32L1XX_MD */
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void){
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void){
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0){
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else{
    bDeviceState = ATTACHED;
  }
    /*Enable SystemCoreClock*/
  SystemInit();//my usb
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void){
  NVIC_InitTypeDef NVIC_InitStructure;
  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //my usb
 
#if defined(STM32L1XX_MD)|| defined(STM32L1XX_HD) || defined(STM32L1XX_MD_PLUS)
  /* Enable the USB interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USB Wake-up interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_FS_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState){
#if defined(STM32L1XX_MD) || defined (STM32L1XX_HD)|| (STM32L1XX_MD_PLUS)
  if (NewState != DISABLE){
    STM32L15_USB_CONNECT;
  }
  else{
    STM32L15_USB_DISCONNECT;
  }
#endif /* STM32L1XX_MD */
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void){
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(uint32_t*)ID1;
  Device_Serial1 = *(uint32_t*)ID2;
  Device_Serial2 = *(uint32_t*)ID3;
 
  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0){
    IntToUnicode (Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
  }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len){
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++){
    if( ((value >> 28)) < 0xA ){
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else{
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    value = value << 4;
    pbuf[ 2* idx + 1] = 0;
  }
}

/*******************************************************************************
* Function Name  : Send DATA .
* Description    : send the data received from the STM32 to the PC through USB  
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint32_t CDC_Send_DATA (uint8_t *ptrBuffer, uint8_t Send_length){
  /*if max buffer is Not reached*/
  if(Send_length < VIRTUAL_COM_PORT_DATA_SIZE){
    /*Sent flag*/
    sUSB_Para.packet_sent = 0; 
    USB_SIL_Write(EP1_OUT,ptrBuffer,Send_length); 
    SetEPTxValid(ENDP1);
  }
  else{
    return 0;
  } 
  return 1;
}

/*******************************************************************************
* Function Name  : Receive DATA .
* Description    : receive the data from the PC to STM32 and send it through USB
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint32_t CDC_Receive_DATA(void){ 
  /*Receive flag*/
  SetEPRxValid(ENDP3); 
  return 1 ;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
