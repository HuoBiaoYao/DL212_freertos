#ifndef __ADS1248_H
#define __ADS1248_H

#include "stm32l1xx_spi.h"
#include "stm32l1xx_gpio.h"

#define SPI_ADS1248           	 SPI1
#define SPI_ADS1248_CLK					 RCC_APB2Periph_SPI1

#define ADS1248_CS_PIN        	 GPIO_Pin_13
#define ADS1248_CS_GPIO_PORT  	 GPIOB
#define ADS1248_CS_GPIO_CLK   	 RCC_AHBPeriph_GPIOB
  
#define ADS1248_GPIO_PORT    	   GPIOA       
#define ADS1248_GPIO_CLK      	 RCC_AHBPeriph_GPIOA 
#define ADS1248_CLK_PIN       	 GPIO_Pin_5   
#define ADS1248_CLK_SOURCE    	 GPIO_PinSource5
#define ADS1248_CLK_AF        	 GPIO_AF_SPI1 
#define ADS1248_SO_PIN         	 GPIO_Pin_6   
#define ADS1248_SO_SOURCE        GPIO_PinSource6
#define ADS1248_SO_AF            GPIO_AF_SPI1 
#define ADS1248_SI_PIN         	 GPIO_Pin_7   
#define ADS1248_SI_SOURCE      	 GPIO_PinSource7
#define ADS1248_SI_AF         	 GPIO_AF_SPI1

#define ADS1248_DISABLE()        GPIO_SetBits(ADS1248_CS_GPIO_PORT, ADS1248_CS_PIN)
#define ADS1248_ENABLE()         GPIO_ResetBits(ADS1248_CS_GPIO_PORT, ADS1248_CS_PIN)
//Conversion start
#define ADS1248_START_GPIO_PIN   GPIO_Pin_12
#define ADS1248_START_GPIO_PORT  GPIOB
#define ADS1248_START_GPIO_CLK   RCC_AHBPeriph_GPIOB
//Data ready (active low)
#define ADS1248_DRDY_GPIO_PIN    GPIO_Pin_14
#define ADS1248_DRDY_GPIO_PORT   GPIOB
#define ADS1248_DRDY_GPIO_CLK    RCC_AHBPeriph_GPIOB
//Reset (active low)
#define ADS1248_RESET_GPIO_PIN   GPIO_Pin_15
#define ADS1248_RESET_GPIO_PORT  GPIOB
#define ADS1248_RESET_GPIO_CLK   RCC_AHBPeriph_GPIOB

#define ADS1248_RST_H()				   ADS1248_RESET_GPIO_PORT->BSRR = ADS1248_RESET_GPIO_PIN
#define ADS1248_RST_L()		       ADS1248_RESET_GPIO_PORT->BRR  = ADS1248_RESET_GPIO_PIN

#define ADS1248_START_H()        ADS1248_START_GPIO_PORT->BSRR = ADS1248_START_GPIO_PIN
#define ADS1248_START_L()        ADS1248_START_GPIO_PORT->BRR  = ADS1248_START_GPIO_PIN

#define IS_ADS1248_READY()       ADS1248_GPIO_PORT->IDR & ADS1248_DRDY_GPIO_PIN 
//#define USE_INTER_VREF
/************************************************************************/
/*  ads1248 register map                                                */
/************************************************************************/
#define ADS_MUX0	 	0X00
#define ADS_VBIAS	 	0X01
#define ADS_MUX1   	0X02
#define ADS_SYS0   	0X03
#define ADS_OFC0	 	0X04
#define ADS_OFC1	 	0X05
#define ADS_OFC2	 	0X06
#define ADS_FSC0		0X07
#define ADS_FSC1		0X08
#define ADS_FSC2		0X09
#define ADS_IDAC0		0X0A
#define ADS_IDAC1		0X0B
#define ADS_GPIOCFG	0X0C
#define ADS_GPIODIR	0X0D
#define ADS_GPIODAT	0X0E

/************************************************************************/
/* ads1248 SPI commands                                                 */
/************************************************************************/
#define ADS_WAKEUP		0x00
#define ADS_SLEEP			0x02
#define ADS_SYNC			0x04
#define ADS_RESET			0x06
#define ADS_NOP				0xFF
#define ADS_RDATA			0x12
#define ADS_RDATAC		0x14
#define ADS_SDATAC		0x16
#define ADS_RREG			0x20
#define ADS_WREG			0x40
#define ADS_SYSOCAL	  0x60
#define ADS_SYSGCAL	  0x61
#define ADS_SELFOCAL	0x62

/************************************************************************/
/* ads1248 macroinstruction                                             */
/************************************************************************/
// about MUX0: Multiplexer Control Register 0
#define  P_AIN0		0x00
#define  P_AIN1		0x08
#define  P_AIN2		0x10
#define  P_AIN3		0x18
#define  P_AIN4		0x20
#define  P_AIN5		0x28
#define  P_AIN6		0x30
#define  P_AIN7		0x38
#define  N_AIN0		0x00
#define  N_AIN1		0x01
#define  N_AIN2		0x02
#define  N_AIN3		0x03
#define  N_AIN4		0x04
#define  N_AIN5		0x05
#define  N_AIN6		0x06
#define  N_AIN7		0x07

// about MUX1: Multiplexer Control Register 1
#define  CLK_Inter						0x00
#define  CLK_Exter						0x80
#define  REF_Inter_AlwaysOn		0x20  // selecte internal reference and always open  
#define  REF_Inter_AlwaysOff	0x00  // selecte internal reference and always off
#define	 SELT_REF0						0x00                               
#define  SELT_REF1						0x08
#define  SELT_Inter						0x10
#define  SELT_Inter_REF0			0x18

// about SYS0 : System Control Register 0
#define	PGAGain_1				0x00
#define	PGAGain_2				0x10
#define	PGAGain_4				0x20
#define	PGAGain_8				0x30
#define	PGAGain_16			0x40
#define	PGAGain_32			0x50
#define	PGAGain_64			0x60
#define	PGAGain_128			0x70
#define	DataRate_5			0x00
#define	DataRate_10			0x01
#define	DataRate_20			0x02
#define	DataRate_40			0x03
#define	DataRate_80			0x04
#define	DataRate_160		0x05
#define	DataRate_320		0x06
#define	DataRate_640		0x07
#define	DataRate_1000		0x08
#define	DataRate_2000		0x09

// about IDAC0: IDAC Control Register 0
#define	Drdy_Mode_EN		0x08
#define	Drdy_Mode_DIS		0x00

// the magnitude of the excitation current.
// The IDACs require the internal reference to be on.
#define	IMAG_Off		0x00
#define	IMAG_50			0x01
#define	IMAG_100		0x02
#define	IMAG_250		0x03
#define	IMAG_500		0x04
#define	IMAG_750		0x05
#define	IMAG_1000		0x06
#define	IMAG_1500		0x07

// about IDAC1: IDAC Control Register 1
// select the output pin for the first current source DAC.
#define	IDAC1_AIN0		0x00
#define	IDAC1_AIN1		0x10
#define	IDAC1_AIN2		0x20
#define	IDAC1_AIN3		0x30
#define	IDAC1_AIN4		0x40
#define	IDAC1_AIN5		0x50
#define	IDAC1_AIN6		0x60
#define	IDAC1_AIN7		0x70
#define	IDAC1_IEXT1		0x80
#define	IDAC1_IEXT2		0x90
#define	IDAC1_OFF			0xC0

// select the output pin for the second current source DAC.
#define	IDAC2_AIN0		0x00
#define	IDAC2_AIN1		0x01
#define	IDAC2_AIN2		0x02
#define	IDAC2_AIN3		0x03
#define	IDAC2_AIN4		0x04
#define	IDAC2_AIN5		0x05
#define	IDAC2_AIN6		0x06
#define	IDAC2_AIN7		0x07
#define	IDAC2_IEXT1		0x08
#define	IDAC2_IEXT2		0x09
#define	IDAC2_OFF			0x0C

void ADS1248_GPIO_Init(void);
void ADS1248_SPI_Init(void); 
void ADS1248_SetIDAC(unsigned char idac1, unsigned char idac2, unsigned char idacImage);
void ADS1248_SetPGAGainAndDataRate(unsigned char pgaGain, unsigned char dataRate);
void ADS1248_SetInputChannel(unsigned char positiveChannel, unsigned char negativeChannel);
void ADS1248_SetReference(unsigned char interVrefOnOff, unsigned char refSelected);
int ADS1248_ReadADC(void);
float ADS1248_ResToTemp(float resistance);
#endif
