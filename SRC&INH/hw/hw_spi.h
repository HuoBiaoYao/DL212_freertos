#ifndef __HW_SPI_H
#define __HW_SPI_H
#include "stm32l1xx_spi.h"

unsigned char SPI_SendByte(SPI_TypeDef* spi,unsigned char byte);
#endif
