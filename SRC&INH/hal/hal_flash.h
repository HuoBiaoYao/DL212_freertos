#ifndef __HAL_FLASH_H
#define __HAL_FLASH_H
 
extern struct _Flash_Func sFlash_Func;
extern struct _Flash_Func *psFlash_Func;

struct _Flash_Func{
  void (*init)(void);
	void (*page_erase)(unsigned int page_addr);
	void (*block_erase)(unsigned int block_addr);
	void (*sector_erase)(unsigned int sector_addr);
	void (*chip_erase)(void);
	void (*write)(unsigned char *data,unsigned int page_addr,unsigned int byte_addr,unsigned int len); 
	void (*read) (unsigned char *data,unsigned int page_addr,unsigned int byte_addr,unsigned int len); 
	void (*test)(void);
	unsigned int (*read_id)(void);
};

void Flash_Test(void);

#endif
