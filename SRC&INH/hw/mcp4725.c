#include "iic.h"
#include "mcp4725.h"

void MCP4725_SetValue(float value){
	unsigned char byte=0;
	unsigned short int dac=0; 
	 
	if(value < 2){
	  value = 0;
	}
	else if(value < 20){
	  value -= 2;
	}
	else if(value < 190){
	  value -= 4;
	}
	else if(value < 310){
	  value -= 2;
	}
	else if(value < 750){
	  value -= 0;
	}
	else if(value < 1100){
	  value -= 2;
	}
	else if(value < 1260){
	  value -= 1;
	}
	else if(value < 1550){
	  value += 1;
	}
	else if(value < 1650){
	  value -= 2;
	}
	else if(value < 1800){
	  value -= 0;
	}
	else if(value < 2400){
	  value += 4;         
	}
	else if(value < 3000){
	  value += 5;
	}
	else{
	  value = 3000;
	} 
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
 