#ifndef __I2C_RTC_H
#define __I2C_RTC_H 

#include "time.h"

#define I2C_RTC_SCK                       RCC_APB1Periph_I2C2

#define I2C_RTC_SCL_PIN                   GPIO_Pin_10                  /* PB.10  */
#define I2C_RTC_SCL_GPIO_PORT             GPIOB                       /* GPIOB */
#define I2C_RTC_SCL_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define I2C_RTC_SCL_SOURCE                GPIO_PinSource10
#define I2C_RTC_SCL_AF                    GPIO_AF_I2C2

#define I2C_RTC_SDA_PIN                   GPIO_Pin_11                  /* PB.11  */
#define I2C_RTC_SDA_GPIO_PORT             GPIOB                       /* GPIOB */
#define I2C_RTC_SDA_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define I2C_RTC_SDA_SOURCE                GPIO_PinSource11
#define I2C_RTC_SDA_AF                    GPIO_AF_I2C2


#define RTC_IRQ_PIN                       GPIO_Pin_2                  /* PB.2  */
#define RTC_IRQ_GPIO_PORT                 GPIOB                       /* GPIOB */
#define RTC_IRQ_GPIO_CLK                  RCC_AHBPeriph_GPIOB
#define RTC_IRQ_SOURCE                    GPIO_PinSource2
#define RTC_IRQ_EXTI_LINE                 EXTI_Line2
#define RTC_IRQ_GPIO_PortSource           EXTI_PortSourceGPIOB
#define RTC_IRQ_EXTI_PIN_SOURCE           EXTI_PinSource2
#define RTC_IRQ_EXTI_IRQn                 EXTI2_IRQn


#define M41T81_WRITEADDR 0xd0
#define M41T81_READADDR  0xd1

static __inline int TimeConv(char time){
  int time1,time2;
    
  time1 = (int)time & 0x0000000f;
  time2 = (int)((time<<24)>>28)*10;
	
  return (time1+time2);
}

extern unsigned int RTC_M41T81_IntCount;

void I2C_RTC_Init (void);
void RTC_EXTI_Init (void); 
void I2C_Write(unsigned char *pBuffer,unsigned int WriteAddr, unsigned int NumByteToWrite);
void I2C_Read (unsigned char *pDat,unsigned int nAddr,unsigned int nLen);
unsigned int TimeConvert(unsigned char *buf,struct tm *pstime);
#endif
