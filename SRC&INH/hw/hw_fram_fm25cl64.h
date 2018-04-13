#ifndef __HW_FRAM_FM25CL64_H
#define __HW_FRAM_FM25CL64_H

#include "stm32l1xx.h" 
//spi1
#define FRAM_SPI                SPI1
#define FRAM_SPI_CLK            RCC_APB2Periph_SPI1


#define FRAM_SPI_CS_PIN         GPIO_Pin_4
#define FRAM_SPI_CS_GPIO_PORT   GPIOC
#define FRAM_SPI_CS_GPIO_CLK    RCC_AHBPeriph_GPIOC

#define FRAM_SPI_CLK_PIN        GPIO_Pin_5  //SPI1-SCK
#define FRAM_SPI_CLK_GPIO_PORT  GPIOA        
#define FRAM_SPI_CLK_GPIO_CLK   RCC_AHBPeriph_GPIOA
#define FRAM_SPI_CLK_SOURCE     GPIO_PinSource5
#define FRAM_SPI_CLK_AF         GPIO_AF_SPI1

#define FRAM_SPI_SO_PIN         GPIO_Pin_6  //SPI1-MISO
#define FRAM_SPI_SO_GPIO_PORT   GPIOA
#define FRAM_SPI_SO_GPIO_CLK    RCC_AHBPeriph_GPIOA
#define FRAM_SPI_SO_SOURCE      GPIO_PinSource6
#define FRAM_SPI_SO_AF          GPIO_AF_SPI1

#define FRAM_SPI_SI_PIN         GPIO_Pin_7  //SPI1-MOSI
#define FRAM_SPI_SI_GPIO_PORT   GPIOA
#define FRAM_SPI_SI_GPIO_CLK    RCC_AHBPeriph_GPIOA
#define FRAM_SPI_SI_SOURCE      GPIO_PinSource7
#define FRAM_SPI_SI_AF          GPIO_AF_SPI1

#define SPI_FRAM_ENABLE()       GPIO_ResetBits(FRAM_SPI_CS_GPIO_PORT, FRAM_SPI_CS_PIN)
#define SPI_FRAM_DISABLE()      GPIO_SetBits(FRAM_SPI_CS_GPIO_PORT, FRAM_SPI_CS_PIN)

//op code
#define WREN  0x06
#define WRDI  0x04
#define RDSR  0x05
#define WRSR  0x01
#define READ  0x03
#define WRITE 0x02
//#define RDID  0x9F

#define DUMMY_BYTE 0xFF 
#define FRAM_CAPACITY  8192  
#define __no_init        __attribute__((zero_init)) 
 
void SPI_FRAM_Init(void);
void SPI_FRAM_Write_Enable(void);
void SPI_FRAM_Write_Disable(void);
void SPI_FRAM_WriteStatusReg(unsigned char command);
unsigned char SPI_FRAM_ReadStatusReg(void);
void SPI_FRAM_WriteByte(unsigned short int addr,unsigned char data);
unsigned char SPI_FRAM_ReadByte(unsigned short int addr);
void SPI_FRAM_WriteBytes(unsigned int addr,unsigned char *data,unsigned int lenth);
void SPI_FRAM_ReadBytes(unsigned int addr,unsigned char *data,unsigned int lenth);
void SPI_FRAM_Erase(unsigned int lenth);

#endif
