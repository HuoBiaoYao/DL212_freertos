#ifndef __DL212_EASY_MODE_H
#define __DL212_EASY_MODE_H

extern unsigned int  C1_Dest,C2_Dest,P_SW_Dest,F_Mea_Dest;
extern unsigned int  C1_Time,C2_Time,P_SW_Time,F_Mea_Time; 
extern unsigned char C1_Option,C2_Option,P_SW_Option,F_Mea_Option;

struct CONFIG{
  char device_id[3];//包含结束符
  unsigned int scan;
  unsigned char mode[5];//差分1，差分2，差分3，DI，D2
  unsigned char sw[11];
  unsigned char rang[6];
  unsigned char vx_sw[6];
  float vx_value[6];
  float mul[10];
  float offset[10];
  unsigned char datatype[4];//周期，频率，计数
  char s0[3];//分隔符//包含结束符    
  char s1[3];//起始符//包含结束符    
  char s2[3];//终止符//包含结束符    
  char sdi12_cmd[2][200];
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

float Battery(void);
float VoltDiff(unsigned char chan,unsigned char range);
float VoltSE(unsigned char chan,unsigned char range);
#endif

