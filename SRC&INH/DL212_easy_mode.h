#ifndef __DL212_EASY_MODE_H
#define __DL212_EASY_MODE_H

struct _SPECIALDATA{
	char addr_0;
	char addr_1;
	unsigned int scan_intvl;
	unsigned int usart_baudrate;
	unsigned char usart_mode;
	unsigned char modbus_address;
  unsigned char v1_func;
	unsigned char v2_func;
	unsigned char v3_func;
	unsigned char v4_func;
	unsigned char f1_func;
	unsigned char d1_func;
	unsigned char d2_func;
	unsigned char sw12_1_func;
	unsigned char sw12_2_func; 
	float value[7];//v1,v2,v3,v4,f1,d1,d2,如果缺省,则后面的参数往前移位
};

extern unsigned char DL212_EasyMode;
extern struct _SPECIALDATA sEMData;

void DL212_EasyMode_Scan(void); 
void DL212_EasyMode_ValueDisplay(void);
void DL212_EasyMode_Init(void);
void DL212_EasyMode_Config(void);
  
#endif

