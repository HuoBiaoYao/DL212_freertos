#include "hal_flash.h" 
#include "hw_flash_at45db.h"

struct _Flash_Func sFlash_Func={
  SPI_FLASH_Init,
	SPI_FLASH_PageErase,
	SPI_FLASH_BlockErase,
	SPI_FLASH_SectorErase,
	SPI_FLASH_ChipErase,
	SPI_FLASH_MainMemoryPageProram_ThroughBuffer1,
	SPI_FLASH_ContinuousArrayRead_HF,
	Flash_Test,
	SPI_FLASH_READID
};
struct _Flash_Func *psFlash_Func=&sFlash_Func;
 
void Flash_Test(void){
	volatile unsigned int i,at45db_id; 
  unsigned char w[0xFF],r[0xFF];
	
	for(i=0;i<0xFF;i++){
	  w[i] = i;
	}  
	at45db_id = psFlash_Func->read_id(); 
	psFlash_Func->write(w,0,0,0xFF);
	psFlash_Func->read (r,0,0,0xFF);
	
	for(i=0;i<0xFF;i++){
	  if(r[i] != w[i]){
		  while(1);
		}
	}
	while(1);
}

 