#ifndef __HW_FLASH_AT45DB_H
#define __HW_FLASH_AT45DB_H

 

//AT45DB321E    32-Mbit
//64sector*64KB
//1024block*4KB
//8192page*528(512)

#define PAGESIZE_MODE     528
#define PAGE_SUM          8192


//spi1
#define FLASH_SPI                SPI1
#define FLASH_SPI_CLK            RCC_APB2Periph_SPI1


#define FLASH_SPI_CS_PIN         GPIO_Pin_5  
#define FLASH_SPI_CS_GPIO_PORT   GPIOC
#define FLASH_SPI_CS_GPIO_CLK    RCC_AHBPeriph_GPIOC
 

#define FLASH_SPI_CLK_PIN        GPIO_Pin_5   
#define FLASH_SPI_CLK_GPIO_PORT  GPIOA       
#define FLASH_SPI_CLK_GPIO_CLK   RCC_AHBPeriph_GPIOA
#define FLASH_SPI_CLK_SOURCE     GPIO_PinSource5
#define FLASH_SPI_CLK_AF         GPIO_AF_SPI1

#define FLASH_SPI_SO_PIN         GPIO_Pin_6  
#define FLASH_SPI_SO_GPIO_PORT   GPIOA
#define FLASH_SPI_SO_GPIO_CLK    RCC_AHBPeriph_GPIOA
#define FLASH_SPI_SO_SOURCE      GPIO_PinSource6
#define FLASH_SPI_SO_AF          GPIO_AF_SPI1

#define FLASH_SPI_SI_PIN         GPIO_Pin_7  
#define FLASH_SPI_SI_GPIO_PORT   GPIOA
#define FLASH_SPI_SI_GPIO_CLK    RCC_AHBPeriph_GPIOA
#define FLASH_SPI_SI_SOURCE      GPIO_PinSource7
#define FLASH_SPI_SI_AF          GPIO_AF_SPI1

#define SPI_FLASH_DISABLE()      GPIO_SetBits(FLASH_SPI_CS_GPIO_PORT, FLASH_SPI_CS_PIN)
#define SPI_FLASH_ENABLE()       GPIO_ResetBits(FLASH_SPI_CS_GPIO_PORT, FLASH_SPI_CS_PIN)
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// 

//---Read Commands
#define MMPAGE_READ                       0xd2
#define CONTINUOUS_ARRAY_READ_LEGCMD      0xe8
#define CONTINUOUS_ARRAY_READ_LOWFREQ     0x03
#define CONTINUOUS_ARRAY_READ_HIGHFREQ    0x0b
#define BUFFER1_READ_LOWFREQ              0xd1
#define BUFFER2_READ_LOWFREQ              0xd3
#define BUFFER1_READ                      0xd4  // buffer 1 read (change to 0xD4 for SPI mode 0,3)
#define BUFFER2_READ                      0xd6  // buffer 2 read (change to 0xD6 for SPI mode 0,3)
//---Program and Erase Commands
#define BUFFER1_WRITE                     0x84  // buffer 1 write
#define BUFFER2_WRITE                     0x87  // buffer 2 write
#define BUFFER1_TO_MMPAGE_WITH_ERASE      0x83  // buffer 1 to main memory page program with built-in erase
#define BUFFER2_TO_MMPAGE_WITH_ERASE      0x86  // buffer 2 to main memory page program with built-in erase
#define BUFFER1_TO_MMPAGE_WITHOUT_ERASE   0x88  // buffer 1 to main memory page program without built-in erase
#define BUFFER2_TO_MMPAGE_WITHOUT_ERASE   0x89  // buffer 2 to main memory page program without built-in erase
#define PAGE_ERASE                        0x81 	 
#define BLOCK_ERASE                       0x50  
#define SECTOR_ERASE                      0x7c
#define MMPAGE_PROG_THROUGH_BUFFER1       0x82  // main memory page program through buffer 1
#define MMPAGE_PROG_THROUGH_BUFFER2       0x85  // main memory page program through buffer 2
//---Additionnal Commands
#define AUTO_PAGE_REWRITE_THROUGH_BUFFER1 0x58  // auto page rewrite through buffer 1
#define AUTO_PAGE_REWRITE_THROUGH_BUFFER2 0x59  // auto page rewrite through buffer 2
#define MMPAGE_TO_BUFFER1_COMPARE         0x60  // main memory page compare to buffer 1
#define MMPAGE_TO_BUFFER2_COMPARE         0x61  // main memory page compare to buffer 2
#define MMPAGE_TO_BUFFER1_TRANSFER        0x53  // main memory page to buffer 1 transfer
#define MMPAGE_TO_BUFFER2_TRANSFER        0x55  // main memory page to buffer 2 transfer
#define DEEP_POWER_DOWN                   0xb9
#define RESUME_FROM_DEEP_POWER_DOWN       0xab
#define STATUS_REGISTER_READ              0xd7
#define MANUFACTURER_AND_DEVICEID_READ    0x9f

#define Dummy_Byte   0xA5
#define WIP_Flag     0x80
 
void SPI_FLASH_Init(void);
void SPI_FLASH_ContinuousArrayRead_HF(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned int NumByteToRead);  
void SPI_FLASH_ContinuousArrayRead_LF(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned short int NumByteToRead);
void SPI_FLASH_MainMemoryRead(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned short int NumByteToRead);
void SPI_FLASH_Buffer1Read(unsigned char* pBuffer, unsigned int ReadAddr, unsigned short int NumByteToRead);
void SPI_FLASH_Buffer2Read(unsigned char* pBuffer, unsigned int ReadAddr, unsigned short int NumByteToRead);
void SPI_FLASH_Buffer1Write(unsigned char* pBuffer, unsigned int WriteAddr, unsigned short int NumByteToWrite);
void SPI_FLASH_Buffer2Write(unsigned char* pBuffer, unsigned int WriteAddr, unsigned short int NumByteToWrite);
void SPI_FLASH_Buffer1ToMMPageProgWithErase(unsigned int PageAddr);
void SPI_FLASH_Buffer2ToMMPageProgWithErase(unsigned int PageAddr);
void SPI_FLASH_MainMemoryPageProram_ThroughBuffer1(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned int NumByteToWrite);
void SPI_FLASH_MainMemoryPageProram_ThroughBuffer2(unsigned char* pBuffer,unsigned int PageAddr,unsigned int ByteAddr,unsigned short int NumByteToWrite);
void SPI_FLASH_PageErase(unsigned int PageAddr);
void SPI_FLASH_BlockErase(unsigned int BlockAddr);
void SPI_FLASH_SectorErase(unsigned int SectorAddr);
void SPI_FLASH_ChipErase(void);
void SPI_FLASH_MMPageToBuffer1(unsigned int PageAddr);
void SPI_FLASH_MMPageToBuffer2(unsigned int PageAddr);
unsigned int SPI_FLASH_READID(void);
void SPI_FLASH_WaitForWriteEnd(void);
 
#endif

