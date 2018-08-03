#ifndef __DL212_EASY_MODE_H
#define __DL212_EASY_MODE_H

struct CONFIG{
  char mark[3];//设备标识
	unsigned int scan;//测量间隔
	unsigned char port[9];//依次为v1,v2,v3,v4,f1,d1,d2,sw12-1,sw12-2
	unsigned char mode;//串口(zigbee)工作方式选择：ascii自动发送或者modbus从机
	unsigned char modbus_addr;//modbus从机地址
	unsigned int baudrate;//串口(zigbee)波特率选择	    
	char sdi12[2][440];//D1口SDI-12指令
	unsigned char lrc;
};

extern unsigned char DL212_Value_Display_Ctrl;
extern unsigned char DL212_EasyMode;
extern struct CONFIG sDL212_Config;
extern float Value[];

void DL212_EasyMode_Scan(void); 
void DL212_EasyMode_ValueDisplay(void);
void DL212_EasyMode_Init(void);
void DL212_EasyMode_Config(void);
 
unsigned char LRC( unsigned char *buf,unsigned short int len);
void DL212_Config_Utility(void);

#endif

