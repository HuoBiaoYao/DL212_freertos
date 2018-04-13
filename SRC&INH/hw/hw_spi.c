#include "hw_spi.h"
 
unsigned char SPI_SendByte(SPI_TypeDef* spi,unsigned char byte){
	unsigned char timeout=0;
	
  while (SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_TXE) == RESET){
	  timeout++;
		if(timeout > 200){
		  return 0;
		}
	}
  SPI_I2S_SendData(spi, byte);
 	timeout = 0; 
	while (SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_RXNE) == RESET){
	  timeout++;
		if(timeout > 200){
		  return 0;
		}
	}
  
  return SPI_I2S_ReceiveData(spi);
}
