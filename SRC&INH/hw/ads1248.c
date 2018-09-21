#include "ads1248.h"

void ADS1248_GPIO_Init(void){
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
	RCC_AHBPeriphClockCmd(ADS1248_CS_GPIO_CLK|ADS1248_GPIO_CLK,ENABLE);
	RCC_AHBPeriphClockCmd(ADS1248_START_GPIO_CLK|ADS1248_DRDY_GPIO_CLK|ADS1248_RESET_GPIO_CLK,ENABLE);
   
	GPIO_InitStructure.GPIO_Pin = ADS1248_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(ADS1248_CS_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinLockConfig(ADS1248_CS_GPIO_PORT,ADS1248_CS_PIN);
	
  GPIO_InitStructure.GPIO_Pin   = ADS1248_CLK_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(ADS1248_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = ADS1248_SO_PIN;
  GPIO_Init(ADS1248_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = ADS1248_SI_PIN;
  GPIO_Init(ADS1248_GPIO_PORT, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(ADS1248_GPIO_PORT, ADS1248_CLK_SOURCE, ADS1248_CLK_AF); 
  GPIO_PinAFConfig(ADS1248_GPIO_PORT, ADS1248_SO_SOURCE,  ADS1248_SO_AF); 
  GPIO_PinAFConfig(ADS1248_GPIO_PORT, ADS1248_SI_SOURCE,  ADS1248_SI_AF);  

  GPIO_InitStructure.GPIO_Pin   = ADS1248_START_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(ADS1248_START_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin   = ADS1248_RESET_GPIO_PIN; 
  GPIO_Init(ADS1248_RESET_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = ADS1248_DRDY_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(ADS1248_DRDY_GPIO_PORT, &GPIO_InitStructure);
	
  ADS1248_DISABLE();
}

void ADS1248_SPI_Init(void){
	SPI_InitTypeDef   SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SPI_ADS1248_CLK, ENABLE); 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_ADS1248, &SPI_InitStructure);
  SPI_Cmd(SPI_ADS1248, ENABLE);
	SPI_CalculateCRC(SPI_ADS1248, ENABLE); 
}
 
static unsigned char ADS1248_SPI_SendByte(unsigned char byte){
	unsigned char timeout=0;
	
  while (SPI_I2S_GetFlagStatus(SPI_ADS1248, SPI_I2S_FLAG_TXE) == RESET){
	  timeout++;
		if(timeout > 200){
		  return 0;
		}
	}
  SPI_I2S_SendData(SPI_ADS1248, byte);
 	timeout = 0; 
	while (SPI_I2S_GetFlagStatus(SPI_ADS1248, SPI_I2S_FLAG_RXNE) == RESET){
	  timeout++;
		if(timeout > 200){
		  return 0;
		}
	}
  
  return SPI_I2S_ReceiveData(SPI_ADS1248);
}

void ADS1248_SetIDAC(unsigned char idac1, unsigned char idac2, unsigned char idacImage){
	ADS1248_SPI_SendByte(ADS_WREG | ADS_IDAC0);
	ADS1248_SPI_SendByte(0x01);
	ADS1248_SPI_SendByte(idacImage);
	ADS1248_SPI_SendByte(idac1 | idac2);
}

void ADS1248_SetPGAGainAndDataRate(unsigned char pgaGain, unsigned char dataRate){
	ADS1248_SPI_SendByte(ADS_WREG | ADS_SYS0);
	ADS1248_SPI_SendByte(0x00);
	ADS1248_SPI_SendByte(pgaGain | dataRate);
}

void ADS1248_SetInputChannel(unsigned char positiveChannel, unsigned char negativeChannel){
	ADS1248_SPI_SendByte(ADS_WREG | ADS_MUX0);
	ADS1248_SPI_SendByte(0x00);
	ADS1248_SPI_SendByte(positiveChannel | negativeChannel);
}

void ADS1248_SetReference(unsigned char interVrefOnOff, unsigned char refSelected){
	ADS1248_SPI_SendByte(ADS_WREG | ADS_MUX1);
	ADS1248_SPI_SendByte(0x00);
	ADS1248_SPI_SendByte(interVrefOnOff | refSelected);
}

unsigned char ADS1248_ReadREG(unsigned char regName, unsigned char byteToRead){
	ADS1248_SPI_SendByte(ADS_RREG | regName);
	if(byteToRead == 1){
		ADS1248_SPI_SendByte(0x00);
	}
	else if(byteToRead == 2){
		ADS1248_SPI_SendByte(0x01);
		ADS1248_SPI_SendByte(0xFF);
	}

	return ADS1248_SPI_SendByte(0xFF);
}

int ADS1248_ReadADC(void){
	unsigned char temp[3];
	int adcVal = 0;
 
	if(IS_ADS1248_READY()){
		return 0;
	}

	ADS1248_SPI_SendByte(ADS_RDATA);
	temp[0] = ADS1248_SPI_SendByte(0xFF);
	temp[1] = ADS1248_SPI_SendByte(0xFF);
	temp[2] = ADS1248_SPI_SendByte(0xFF);

	adcVal = (int)(temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8); 

	return (adcVal >> 8); // ??8?????
}
