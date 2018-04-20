#include "hw_rtc_m41t81.h"
#include "stm32l1xx_i2c.h"

const char Buf_00h_Alarm[20]={0x00,0x05,0x45,0x10,0x04,0x23,0x05,0x13,0x80,0x00,
							                0x80,0xC0,0x80,0x80,0x81,0x00,0x00,0x00,0x00,0x00};    
const	char Buf_09h_Alarm[11]={0x00,0x80,0xC0,0x80,0x80,0x81,0x00,0x00,0x00,0x00,0x00};//1Hz报警输出
	                           //wdog,alm, ald, alh, alm, als, flgs 
const	char Buf_09h_Wdog[11]={0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //看门狗输出
	                         //wdog,alm, ald, alh, alm, als, flgs 
const char Buf_00h_Sqwe[20]={0x00,0x05,0x45,0x15,0x07,0x23,0x03,0x15,0x80,0x00,
                             0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0};          //1hz方波输出
const char Buf_08h_Sqwe[12]={0x86,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0};//000110校准,1HZ方波输出

unsigned int RTC_M41T81_IntCount=0;

void I2C_RTC_Init(void){
  unsigned int i;	
  GPIO_InitTypeDef  GPIO_InitStructure;
  I2C_InitTypeDef   I2C_InitStructure;
	
	RCC_AHBPeriphClockCmd(I2C_RTC_SCL_GPIO_CLK |I2C_RTC_SCL_GPIO_CLK|RTC_IRQ_GPIO_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  
  //!< Configure I2C2 pins: SCL 
  GPIO_InitStructure.GPIO_Pin = I2C_RTC_SCL_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
  GPIO_Init(I2C_RTC_SCL_GPIO_PORT, &GPIO_InitStructure); 
  //!< Configure I2C2 pins: SDA 
  GPIO_InitStructure.GPIO_Pin = I2C_RTC_SDA_PIN; 
  GPIO_Init(I2C_RTC_SDA_GPIO_PORT, &GPIO_InitStructure); 
 
	GPIO_PinAFConfig(I2C_RTC_SCL_GPIO_PORT, I2C_RTC_SCL_SOURCE, I2C_RTC_SCL_AF);
  GPIO_PinAFConfig(I2C_RTC_SDA_GPIO_PORT, I2C_RTC_SDA_SOURCE, I2C_RTC_SDA_AF);
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//I2C快速模式
  I2C_InitStructure.I2C_OwnAddress1 = 0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
  I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2, ENABLE);
	I2C_AcknowledgeConfig(I2C2,ENABLE);
	 
	for(i=0;i<100000;i++);
	//I2C_Write((unsigned char *)Buf_08h_Sqwe,0x08,12);
  I2C_Write((unsigned char *)Buf_00h_Sqwe,0x0,20);//芯片第一次配置时使用
	for(i=0;i<100000;i++);
	RTC_EXTI_Init();
}

void RTC_EXTI_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RTC_IRQ_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = RTC_IRQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
  GPIO_Init(RTC_IRQ_GPIO_PORT,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	SYSCFG_EXTILineConfig(RTC_IRQ_GPIO_PortSource,RTC_IRQ_EXTI_PIN_SOURCE);
	EXTI_InitStructure.EXTI_Line = RTC_IRQ_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQ_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
 
void I2C_Write(unsigned char* pBuffer, unsigned int WriteAddr, unsigned int NumByteToWrite){
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)){};
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); 
  I2C_Send7bitAddress(I2C2,M41T81_WRITEADDR,I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  
  I2C_SendData(I2C2, WriteAddr);  
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  while(NumByteToWrite--)  {
    I2C_SendData(I2C2, *pBuffer); 
    pBuffer++; 
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }
  I2C_GenerateSTOP(I2C2, ENABLE);
}

void I2C_Read(unsigned char *pDat,unsigned int nAddr,unsigned int nLen){
  unsigned short int i = 0;
  /* Enable I2C2 acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(I2C2, ENABLE);
  /*----- Transmission Phase -----*/
  /* Send I2C2 START condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  /* Test on I2C2 EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); /* EV5 */
  /* Send 24CXX slave address for write */
  I2C_Send7bitAddress(I2C2, M41T81_WRITEADDR, I2C_Direction_Transmitter);
  /* Test on I2C2 EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); /* EV6 */

/*// Send the high part of memory address 
  I2C_SendData(I2C2, (u8)(nAddr>>8));
  // Test on I2C2 EV8 and clear it
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) // EV8
  {
  }
*/
  /* Send the low part of memory address */
  I2C_SendData(I2C2, (unsigned char)nAddr);
  /* Test on I2C2 EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); /* EV8 */
  /*----- Reception Phase -----*/
  /* Send Re-STRAT condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));  /* EV5 */
  /* Send 24CXX slave address for read */
  I2C_Send7bitAddress(I2C2, M41T81_WRITEADDR, I2C_Direction_Receiver);
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));  /* EV6 */
  for(i=0;i<nLen;i++){
    if((i+1)>=nLen){
      /* Disable I2C2 acknowledgement */
      I2C_AcknowledgeConfig(I2C2, DISABLE);
      /* Send I2C2 STOP Condition */
      I2C_GenerateSTOP(I2C2, ENABLE);
    }
    /* Test on EV7 and clear it */
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));  /* EV7 */
    /* Store I2C2 received data */
    *(pDat+i) = I2C_ReceiveData(I2C2);
  }
}

//读取RTC,并将时间处理为与1970.1.1.0.0.0的时间差,四个字节表示
unsigned int TimeConvert(unsigned char *buf,struct tm *pstime){ 
	I2C_Read(buf,0,8);
  //AT91F_TWI_Read(AT91C_BASE_TWI,0,RtcRead,8);
  *(buf+7) = TimeConv(*(buf+7));//年
  *(buf+6) = TimeConv(*(buf+6));//月
  *(buf+5) = TimeConv(*(buf+5));//日
  *(buf+4) = TimeConv(*(buf+4));//星期
  *(buf+3) = TimeConv(*(buf+3))-8;//时
  *(buf+2) = TimeConv(*(buf+2));//分
  *(buf+1) = TimeConv(*(buf+1));//秒
  *(buf+0) = TimeConv(*(buf+0));//毫秒
  //Time_Seconds = *(buf+1)+*(buf+2)*60+*(buf+3)*3600;//将一天当中的时间点换算成秒
  pstime->tm_year  = *(buf+7)+100; 
	pstime->tm_mon   = *(buf+6)-1; 
	pstime->tm_mday  = *(buf+5);
	pstime->tm_wday  = *(buf+4);
	pstime->tm_hour  = *(buf+3); 
	pstime->tm_min   = *(buf+2); 
	pstime->tm_sec   = *(buf+1); 
	pstime->tm_isdst = 0;
	if(pstime->tm_hour < 0){
	  pstime->tm_hour += 24;
	  pstime->tm_mday++;
	} 
	
	return mktime(pstime); 
}