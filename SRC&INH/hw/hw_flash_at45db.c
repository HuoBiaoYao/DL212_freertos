#include "hw_spi.h"
#include "hw_flash_at45db.h" 
 
 
void SPI_FLASH_Init(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
	
	RCC_AHBPeriphClockCmd(FLASH_SPI_CS_GPIO_CLK|FLASH_SPI_CLK_GPIO_CLK|FLASH_SPI_SO_GPIO_CLK|FLASH_SPI_SI_GPIO_CLK,ENABLE);
  RCC_APB2PeriphClockCmd(FLASH_SPI_CLK, ENABLE); 
  
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(FLASH_SPI_CS_GPIO_PORT, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin   = FLASH_SPI_CLK_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(FLASH_SPI_CLK_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SO_PIN;
  GPIO_Init(FLASH_SPI_SO_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SI_PIN;
  GPIO_Init(FLASH_SPI_SO_GPIO_PORT, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(FLASH_SPI_CLK_GPIO_PORT, FLASH_SPI_CLK_SOURCE, FLASH_SPI_CLK_AF);
  GPIO_PinAFConfig(FLASH_SPI_SO_GPIO_PORT,  FLASH_SPI_SO_SOURCE,  FLASH_SPI_SO_AF); 
  GPIO_PinAFConfig(FLASH_SPI_SI_GPIO_PORT,  FLASH_SPI_SI_SOURCE,  FLASH_SPI_SI_AF);  

  SPI_FLASH_DISABLE();
	 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPI, &SPI_InitStructure);
  SPI_Cmd(FLASH_SPI, ENABLE);
	SPI_CalculateCRC(FLASH_SPI, ENABLE); 
}
//-----------------------------------------------------//
//-------------------Read Commands---------------------//
//-----------------------------------------------------//
void SPI_FLASH_ContinuousArrayRead_HF(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned int NumByteToRead){  
	SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
	SPI_SendByte(FLASH_SPI,CONTINUOUS_ARRAY_READ_HIGHFREQ);
#if PAGESIZE_MODE == 528
	SPI_SendByte(FLASH_SPI,PageAddr>>6);
  SPI_SendByte(FLASH_SPI,(PageAddr<<2)|(ByteAddr>>8));
	SPI_SendByte(FLASH_SPI,ByteAddr);
	SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
	while(NumByteToRead--){
	  *pBuffer = SPI_SendByte(FLASH_SPI,Dummy_Byte);
    pBuffer++;
	}
	SPI_FLASH_DISABLE();
}

void SPI_FLASH_ContinuousArrayRead_LF(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned short int NumByteToRead){ 
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();	  
	SPI_SendByte(FLASH_SPI,CONTINUOUS_ARRAY_READ_LOWFREQ);
#if PAGESIZE_MODE == 528
	SPI_SendByte(FLASH_SPI,PageAddr>>6);
  SPI_SendByte(FLASH_SPI,(PageAddr<<2)|(ByteAddr>>8));
	SPI_SendByte(FLASH_SPI,ByteAddr);
#elif PAGESIZE_MODE == 512
#endif
	while(NumByteToRead--){
	  *pBuffer = SPI_SendByte(FLASH_SPI,Dummy_Byte);
    pBuffer++;
	}
	SPI_FLASH_DISABLE();
}

void SPI_FLASH_MainMemoryRead(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned short int NumByteToRead){
  unsigned char i;
	
	SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
	SPI_SendByte(FLASH_SPI,MMPAGE_READ);
#if PAGESIZE_MODE == 528
	SPI_SendByte(FLASH_SPI,PageAddr>>6);
  SPI_SendByte(FLASH_SPI,(PageAddr<<2)|(ByteAddr>>8));
	SPI_SendByte(FLASH_SPI,ByteAddr);
#elif PAGESIZE_MODE == 512
#endif
	for(i=0;i<4;i++){
	  SPI_SendByte(FLASH_SPI,Dummy_Byte);
	}
	while(NumByteToRead--){
	  *pBuffer = SPI_SendByte(FLASH_SPI,Dummy_Byte);
    pBuffer++;
	}
	SPI_FLASH_DISABLE();
}
void SPI_FLASH_Buffer1Read(unsigned char* pBuffer, unsigned int ReadAddr, unsigned short int NumByteToRead){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,BUFFER1_READ);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
  SPI_SendByte(FLASH_SPI,ReadAddr>>8);
  SPI_SendByte(FLASH_SPI,ReadAddr);
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
  while (NumByteToRead--){
    *pBuffer = SPI_SendByte(FLASH_SPI,Dummy_Byte);
    pBuffer++;
  }
  SPI_FLASH_DISABLE();
}
void SPI_FLASH_Buffer2Read(unsigned char* pBuffer, unsigned int ReadAddr, unsigned short int NumByteToRead){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,BUFFER2_READ);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
  SPI_SendByte(FLASH_SPI,ReadAddr>>8);
  SPI_SendByte(FLASH_SPI,ReadAddr);
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
  while (NumByteToRead--){
    *pBuffer = SPI_SendByte(FLASH_SPI,Dummy_Byte);
    pBuffer++;
  }
  SPI_FLASH_DISABLE();
}
//-----------------------------------------------------//
//------------Praogram and Erase Commands--------------//
//-----------------------------------------------------//
void SPI_FLASH_Buffer1Write(unsigned char* pBuffer, unsigned int WriteAddr, unsigned short int NumByteToWrite){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,BUFFER1_WRITE);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
  SPI_SendByte(FLASH_SPI,(WriteAddr&0x0000FF00) >> 8);
  SPI_SendByte(FLASH_SPI,WriteAddr&0x000000FF);
#elif PAGESIZE_MODE == 512
#endif
  while(NumByteToWrite--){
    SPI_SendByte(FLASH_SPI,*pBuffer);
    pBuffer++;
  }
  SPI_FLASH_DISABLE();
}
void SPI_FLASH_Buffer2Write(unsigned char* pBuffer, unsigned int WriteAddr, unsigned short int NumByteToWrite){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,BUFFER2_WRITE);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
  SPI_SendByte(FLASH_SPI,(WriteAddr&0x0000FF00) >> 8);
  SPI_SendByte(FLASH_SPI,WriteAddr&0x000000FF);
#elif PAGESIZE_MODE == 512
#endif
  while (NumByteToWrite--){
    SPI_SendByte(FLASH_SPI,*pBuffer);
    pBuffer++;
  }
  SPI_FLASH_DISABLE();
}
void SPI_FLASH_Buffer1ToMMPageProgWithErase(unsigned int PageAddr){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,BUFFER1_TO_MMPAGE_WITH_ERASE);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,(PageAddr )>>6);
  SPI_SendByte(FLASH_SPI,(PageAddr )<<2);
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
  SPI_FLASH_DISABLE();
}
void SPI_FLASH_Buffer2ToMMPageProgWithErase(unsigned int PageAddr){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,BUFFER2_TO_MMPAGE_WITH_ERASE);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,(PageAddr )>>6);
  SPI_SendByte(FLASH_SPI,(PageAddr )<<2);
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
  SPI_FLASH_DISABLE();
}
void SPI_FLASH_MainMemoryPageProram_ThroughBuffer1(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned int NumByteToWrite){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
	SPI_SendByte(FLASH_SPI,MMPAGE_PROG_THROUGH_BUFFER1);
#if PAGESIZE_MODE == 528	
	SPI_SendByte(FLASH_SPI,PageAddr>>6);
  SPI_SendByte(FLASH_SPI,(PageAddr<<2)|(ByteAddr>>8));
	SPI_SendByte(FLASH_SPI,ByteAddr);
#elif PAGESIZE_MODE == 512
#endif
	while(NumByteToWrite--){
    SPI_SendByte(FLASH_SPI,*pBuffer);
    pBuffer++;
	}
	SPI_FLASH_DISABLE();
}
void SPI_FLASH_MainMemoryPageProram_ThroughBuffer2(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned short int NumByteToWrite){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
	SPI_SendByte(FLASH_SPI,MMPAGE_PROG_THROUGH_BUFFER2);
#if PAGESIZE_MODE == 528	
	SPI_SendByte(FLASH_SPI,PageAddr>>6);
  SPI_SendByte(FLASH_SPI,(PageAddr<<2)|(ByteAddr>>8));
	SPI_SendByte(FLASH_SPI,ByteAddr);
#elif PAGESIZE_MODE == 512
#endif
	while(NumByteToWrite--){
    SPI_SendByte(FLASH_SPI,*pBuffer);
    pBuffer++;
	}
	SPI_FLASH_DISABLE();
}
void SPI_FLASH_PageErase(unsigned int PageAddr){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,PAGE_ERASE);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,PageAddr>>6);
  SPI_SendByte(FLASH_SPI,PageAddr<<2);
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
  SPI_FLASH_DISABLE();
}
void SPI_FLASH_BlockErase(unsigned int BlockAddr){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,BLOCK_ERASE);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,BlockAddr>>3);
  SPI_SendByte(FLASH_SPI,BlockAddr<<7);
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
  SPI_FLASH_DISABLE();
}
void SPI_FLASH_SectorErase(unsigned int SectorAddr){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,SECTOR_ERASE);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,SectorAddr<<1);
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
  SPI_FLASH_DISABLE();
}
void SPI_FLASH_ChipErase(void){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,0xc7);
  SPI_SendByte(FLASH_SPI,0x94);
  SPI_SendByte(FLASH_SPI,0x80);
  SPI_SendByte(FLASH_SPI,0x9a);
  SPI_FLASH_DISABLE();
}
//-----------------------------------------------------//
//-------------------Additionnal Commands--------------//
//-----------------------------------------------------//
void SPI_FLASH_MMPageToBuffer1(unsigned int PageAddr){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,MMPAGE_TO_BUFFER1_TRANSFER);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,PageAddr>>6);
  SPI_SendByte(FLASH_SPI,PageAddr<<2);
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
  SPI_FLASH_DISABLE();
}
void SPI_FLASH_MMPageToBuffer2(unsigned int PageAddr){
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,MMPAGE_TO_BUFFER2_TRANSFER);
#if PAGESIZE_MODE == 528	
  SPI_SendByte(FLASH_SPI,PageAddr>>6);
  SPI_SendByte(FLASH_SPI,PageAddr<<2); 
  SPI_SendByte(FLASH_SPI,Dummy_Byte);
#elif PAGESIZE_MODE == 512
#endif
  SPI_FLASH_DISABLE();
}
unsigned int SPI_FLASH_READID(void){
  unsigned int temp[5],flash_id;
 
	SPI_FLASH_WaitForWriteEnd(); 
  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,MANUFACTURER_AND_DEVICEID_READ);
  temp[0] = SPI_SendByte(FLASH_SPI,Dummy_Byte);
  temp[1] = SPI_SendByte(FLASH_SPI,Dummy_Byte); 
  temp[2] = SPI_SendByte(FLASH_SPI,Dummy_Byte);
  temp[3] = SPI_SendByte(FLASH_SPI,Dummy_Byte);//可选
	temp[4] = SPI_SendByte(FLASH_SPI,Dummy_Byte);//可选
  SPI_FLASH_DISABLE();
  flash_id = (temp[0] << 24) | (temp[1] << 16) | (temp[2]<<8) | temp[3];//不算最后一个，否则超出4个字节了
  if(0x1f270101 == flash_id){
	  return flash_id;
	} 
	else{
	  return 0;  
	}
}
//-----------------------------------------------------//
//-------------------状态判断--------------------------//
//-----------------------------------------------------//
void SPI_FLASH_WaitForWriteEnd(void){
  unsigned char FLASH_Status = 0;

  SPI_FLASH_ENABLE();
  SPI_SendByte(FLASH_SPI,STATUS_REGISTER_READ);
  do{
    FLASH_Status = SPI_SendByte(FLASH_SPI,Dummy_Byte);
  }
  while((FLASH_Status & WIP_Flag) == RESET);
  SPI_FLASH_DISABLE();
}
 
