#include "iic.h"
#include "mcp4725.h"

void MCP4725_SetValue(float value){
	unsigned char byte=0;
	unsigned short int dac=0; 
 
	value /= 2; 
	dac = value*4096/3000;
  IIC_Start();
	if(!(IIC_Write_Byte(MCP4725_WRITE_ADDRESS))){
			byte = dac>>8&0x0F;
		  if(!(IIC_Write_Byte(byte))){
				byte = dac;
				if(!(IIC_Write_Byte(byte))){
				  __nop();	
				}
			}
	}
	IIC_Stop();
  __nop();	
}
 