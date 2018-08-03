#ifndef __DL212_EASY_MODE_H
#define __DL212_EASY_MODE_H

struct CONFIG{
  char mark[3];//�豸��ʶ
	unsigned int scan;//�������
	unsigned char port[9];//����Ϊv1,v2,v3,v4,f1,d1,d2,sw12-1,sw12-2
	unsigned char mode;//����(zigbee)������ʽѡ��ascii�Զ����ͻ���modbus�ӻ�
	unsigned char modbus_addr;//modbus�ӻ���ַ
	unsigned int baudrate;//����(zigbee)������ѡ��	    
	char sdi12[2][440];//D1��SDI-12ָ��
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

