#ifndef __HW_FLASH_H
#define __HW_FLASH_H
  
#if defined(STM32L1XX_MD)

#define EEPROM_BANK_START_ADDR 0x08080000
#define EEPROM_BANK_END_ADDR   0x08080800
#define STM32_PAGE_SIZE       128 	 		  //字节
#define STM32_SECTOR_SIZE     1024        //字节
	
#elif defined(STM32L1XX_HD)
//data bank 1 0x08080000-0x080817FF
//data bank 2 0x08081800-0x08082FFF

#define EEPROM_BANK_START_ADDR 0x08080000
#define EEPROM_BANK_END_ADDR   0x080817FF
#define STM32_PAGE_SIZE       128 	 		  //字节
#define STM32_SECTOR_SIZE     1024        //字节

#endif
	
unsigned char EEPROM_Write(unsigned char *buf,unsigned int addr,unsigned int len);
unsigned char EEPROM_Read (unsigned char* buf,unsigned int addr,unsigned int len);

unsigned int HW_STM32_Flash_Read_Word(unsigned int addr);
unsigned int HW_STM32_Flash_Read(unsigned int addr,unsigned int *pBuffer,unsigned int num_of_byte);
unsigned int HW_STM32_Flash_Write_NoCheck(unsigned int addr,unsigned int *pBuffer,unsigned int num_of_byte);
unsigned int HW_STMFlash_Erase(unsigned int addr,unsigned int num_of_byte);

void Test_Write(void);
#endif
