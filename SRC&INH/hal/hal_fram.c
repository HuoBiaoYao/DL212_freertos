#include "hal_fram.h"
#include "hw_fram_fm25cl64.h"

struct _Fram_Func sFram_Func={
  SPI_FRAM_Init,
	SPI_FRAM_Erase,
	SPI_FRAM_WriteBytes,
	SPI_FRAM_ReadBytes,
	Fram_Test
};
struct _Fram_Func *psFram_Func=&sFram_Func;

void Fram_Test(void){
	unsigned int i;
  unsigned char w[0xFF],r[0xFF];
	
	for(i=0;i<0xFF;i++){
	  w[i] = i;
	}
	psFram_Func->write(0,w,0xFF);
	psFram_Func->read (0,r,0xFF);
	
	for(i=0;i<0xFF;i++){
	  if(r[i] != w[i]){
		  while(1);
		}
	}
	while(1);
}

 