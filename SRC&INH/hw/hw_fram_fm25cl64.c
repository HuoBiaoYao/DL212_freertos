#include "hw_spi.h"   
#include "hw_fram_fm25cl64.h"

//FM25CL64B-GTR  8192*8    0x0000-0x2000
/**********************************************************
   fram address | fram data
         0x0000--- 
         0x0020--- 
         0x0040--- 
         0x0060--- 
         0x0064--- 
				 0x0068--- 
				 0x006C--- 
         0x006D--- 
         0x0071--- 
         0x0072--- 
         0x0076--- 
         0x0000--- 
         0x0100---用于表一数据缓存，满528字节后写入flash 	
         0x0520---用于表二数据缓存，满528字节后写入flash
				 0x0940---用于存储测量校准参数,共1000个float型数据
***********************************************************/
 
void SPI_FRAM_Erase(unsigned int lenth){
	unsigned short int i;
	
  SPI_FRAM_Write_Enable();
	SPI_FRAM_ENABLE();
	SPI_SendByte(FRAM_SPI,WRITE);
	SPI_SendByte(FRAM_SPI,(0<<3)>>5);
  SPI_SendByte(FRAM_SPI,0<<11);
	for(i=0;i<lenth;i++){
	  SPI_SendByte(FRAM_SPI,0);
	}
	SPI_FRAM_DISABLE();
}

void SPI_FRAM_Init(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(FRAM_SPI_CLK,ENABLE);
  
	GPIO_InitStructure.GPIO_Pin = FRAM_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(FRAM_SPI_CS_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin   = FRAM_SPI_CLK_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(FRAM_SPI_CLK_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = FRAM_SPI_SO_PIN;
  GPIO_Init(FRAM_SPI_SO_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FRAM_SPI_SI_PIN;
  GPIO_Init(FRAM_SPI_SO_GPIO_PORT, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(FRAM_SPI_CLK_GPIO_PORT, FRAM_SPI_CLK_SOURCE, FRAM_SPI_CLK_AF);
  GPIO_PinAFConfig(FRAM_SPI_SO_GPIO_PORT,  FRAM_SPI_SO_SOURCE,  FRAM_SPI_SO_AF); 
  GPIO_PinAFConfig(FRAM_SPI_SI_GPIO_PORT,  FRAM_SPI_SI_SOURCE,  FRAM_SPI_SI_AF);  
 
  SPI_FRAM_ENABLE();
	 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FRAM_SPI, &SPI_InitStructure);
  SPI_Cmd(FRAM_SPI, ENABLE);
	SPI_CalculateCRC(FRAM_SPI, ENABLE);	
}

void SPI_FRAM_Write_Enable(void){
	SPI_FRAM_ENABLE();
  SPI_SendByte(FRAM_SPI,WREN);
	SPI_FRAM_DISABLE();
}
void SPI_FRAM_Write_Disable(void){
  SPI_FRAM_ENABLE();
  SPI_SendByte(FRAM_SPI,WRDI);
	SPI_FRAM_DISABLE();
}

void SPI_FRAM_WriteStatusReg(unsigned char command){
  SPI_FRAM_Write_Enable();
	SPI_FRAM_ENABLE();
	SPI_SendByte(FRAM_SPI,WRSR);
	SPI_SendByte(FRAM_SPI,command);
	SPI_FRAM_DISABLE();
}

unsigned char SPI_FRAM_ReadStatusReg(void){
	unsigned char status;
	
  SPI_FRAM_Write_Enable();
	SPI_FRAM_ENABLE();
	SPI_SendByte(FRAM_SPI,RDSR);
	status = SPI_SendByte(FRAM_SPI,DUMMY_BYTE);
	SPI_FRAM_DISABLE();
	
	return status;
}
//向FRAM指定地址中写入一个字节
void SPI_FRAM_WriteByte(unsigned short int addr,unsigned char data){
  SPI_FRAM_Write_Enable();
	SPI_FRAM_ENABLE();
	SPI_SendByte(FRAM_SPI,WRITE);
	SPI_SendByte(FRAM_SPI,addr>>8);
  SPI_SendByte(FRAM_SPI,addr<<8>>8);	
	SPI_SendByte(FRAM_SPI,data);
	SPI_FRAM_DISABLE();
}
//从FRAM指定地址中读取一个字节
unsigned char SPI_FRAM_ReadByte(unsigned short int addr){
	unsigned char data;
	
  SPI_FRAM_Write_Enable();
	SPI_FRAM_ENABLE();
	SPI_SendByte(FRAM_SPI,READ);
	SPI_SendByte(FRAM_SPI,addr>>8);
  SPI_SendByte(FRAM_SPI,addr<<8>>8);	
	data = SPI_SendByte(FRAM_SPI,DUMMY_BYTE);
	SPI_FRAM_DISABLE();
	
	return data;
}
//向FRAM写入一定长度的数据
void SPI_FRAM_WriteBytes(unsigned int addr,unsigned char *data,unsigned int lenth){
  SPI_FRAM_Write_Enable();
	SPI_FRAM_ENABLE();
	SPI_SendByte(FRAM_SPI,WRITE);
	SPI_SendByte(FRAM_SPI,addr>>8);
  SPI_SendByte(FRAM_SPI,addr<<8>>8);
	//SPI_SendByte(FRAM_SPI,(addr<<3)>>5);
  //SPI_SendByte(FRAM_SPI,addr<<11);	
	while(lenth--){
	  SPI_SendByte(FRAM_SPI,*(data++));
	}
	SPI_FRAM_DISABLE();
}

//从FRAM读取一定长度的数据
void SPI_FRAM_ReadBytes(unsigned int addr,unsigned char *data,unsigned int lenth){
  SPI_FRAM_Write_Enable();
	SPI_FRAM_ENABLE();
	SPI_SendByte(FRAM_SPI,READ);
	SPI_SendByte(FRAM_SPI,addr>>8);
  SPI_SendByte(FRAM_SPI,addr<<8>>8);
	//SPI_SendByte(FRAM_SPI,(addr<<3)>>5);
  //SPI_SendByte(FRAM_SPI,addr<<11);	
	while(lenth--){
	  *(data++) = SPI_SendByte(FRAM_SPI,DUMMY_BYTE);
	}
	SPI_FRAM_DISABLE();
}

