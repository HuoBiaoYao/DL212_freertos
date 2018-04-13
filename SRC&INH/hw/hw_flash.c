#include "hw_flash.h"
#include "stm32l1xx_flash.h"

FLASH_Status FLASHStatus; 
unsigned int FirstPage = 0, NbOfPages = 0, Address = 0;

unsigned char EEPROM_Write(unsigned char *buf,unsigned int addr,unsigned int len){
	unsigned int i;
	
  DATA_EEPROM_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_SIZERR|FLASH_FLAG_OPTVERR|FLASH_FLAG_OPTVERRUSR);
	for(i=0;i<len;i++){
	  FLASHStatus = DATA_EEPROM_ProgramByte(addr+i,*(buf+i));
		if(FLASHStatus != FLASH_COMPLETE){
		  return 1;
		}
	}
	DATA_EEPROM_Lock();
	
	return 0;
}
 
unsigned char EEPROM_Read(unsigned char* buf,unsigned int addr,unsigned int len){
  unsigned int i=0; 
	
	for(i=0;i<len;i++){
	  *(unsigned char*)(buf+i) = *(volatile unsigned int*)(addr+i);
	} 
	
	return 0;
}

unsigned int HW_STM32_Flash_Read_Word(unsigned int addr){
	return *(volatile unsigned int*)addr; 
}
unsigned int STM32_Flash_Read(unsigned int addr,unsigned int *pBuffer,unsigned int num_of_byte){
	unsigned int i;
	
	if(num_of_byte%4 != 0){
	  num_of_byte /= 4;
	}
	else{
	  num_of_byte = num_of_byte/4+1;
	}
	for(i=0;i<num_of_byte;i++){
		pBuffer[i] = HW_STM32_Flash_Read_Word(addr);
		addr += 4;
	}
	
	return 0;
}

unsigned int HW_STM32_Flash_Write_NoCheck(unsigned int addr,unsigned int *pBuffer,unsigned int num_of_byte){
	unsigned int i;
	
	for(i=0;i<num_of_byte;i++){
		FLASH_FastProgramWord( addr,pBuffer[i]);
    addr += 4;
	}
	
	return 0;
}
 
unsigned int HW_STM32_Flash_Erase(unsigned int addr,unsigned int num_of_byte){
	unsigned int i;
	
	if(num_of_byte%STM32_PAGE_SIZE != 0){
	  NbOfPages = num_of_byte/STM32_PAGE_SIZE+1;
	}
  else{
    NbOfPages = num_of_byte/STM32_PAGE_SIZE;
  }
	FLASH_Unlock();
	for(i=0;i<NbOfPages;i++){
    FLASH_ErasePage(addr+i*STM32_PAGE_SIZE);
  }
	FLASH_Lock();
	
	return 0;
}

unsigned int HW_STM32_Flash_App_Write(unsigned int addr,unsigned int *pBuffer,unsigned int num_of_byte){            	                 
	//假如最后剩下3个字节 aa ab 02,写入是4字节一写,一个不存在的字节将被写入flash，会不会有问题 ?

	//__disable_irq();
  FLASH_Unlock(); //解锁
	while(num_of_byte > STM32_SECTOR_SIZE){
	  HW_STM32_Flash_Write_NoCheck(addr,pBuffer,STM32_SECTOR_SIZE/4); 	
		num_of_byte -= STM32_SECTOR_SIZE;
	}
	if(num_of_byte%4 != 0){
	  num_of_byte = num_of_byte/4+1;
	}
	else{
	  num_of_byte = num_of_byte/4;
	}
	HW_STM32_Flash_Write_NoCheck(addr,pBuffer,num_of_byte); 
  FLASH_Lock(); //上锁
  //__enable_irq();
	
	return 0;
}
  
void Test_Write(void){
  unsigned int buf[256];
	unsigned int i;
 
	for(i=0;i<256;i++){
	  buf[i] = 0xAABBCCDD;
	}
	HW_STM32_Flash_Erase(0x8000000,256*4); 
	HW_STM32_Flash_App_Write(0x8000000,buf,256*4);
	STM32_Flash_Read(0x8000000,buf,256*4);
	for(i=0;i<256;i++){
	  buf[i] = 0xEEEEEEEE;
	}
	
	for(i=0;i<256;i++){
	  if(buf[i] != 0xAABBCCDD){
	    __NOP();//UsartSend((unsigned char*)&buf[i],4);
		}
	}
}


