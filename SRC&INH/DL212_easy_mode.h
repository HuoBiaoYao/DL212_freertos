#ifndef __DL212_EASY_MODE_H
#define __DL212_EASY_MODE_H
 
#define VALUE_DISPLAY        0
#define SDI12_0_TRANSPARENT  1
#define SDI12_1_TRANSPARENT  2

struct CONFIG{
  char device_id[3];//包含结束符
  unsigned int scan;
  unsigned char debug_mode; 	
  unsigned char mode[5];//差分1，差分2，差分3，DI，D2
  unsigned char sw[11];
  unsigned char range[6];
  unsigned char vx_sw[6];
  float vx_value[6];
	unsigned int mea_time[4];
  float mul[10];
  float offset[10];
  unsigned char datatype[4];//周期，频率，计数
  char s0[3];//分隔符//包含结束符    
  char s1[3];//起始符//包含结束符    
  char s2[3];//终止符//包含结束符    
  char sdi12_cmd[2][200];
  unsigned char lrc;
};

extern unsigned char DL212_DebugMode;
extern struct CONFIG sDL212_Config;
extern float PSW_Value,PLL_Value,C1_Value,C2_Value;
extern float Value[];
extern unsigned int RTC_IntCount;

float Battery(void);
float VoltDiff(unsigned char chan,unsigned char range);
float VoltSe(unsigned char chan,unsigned char range);

void DL212_EasyMode_Scan(void);  
void DL212_EasyMode_Init(void);
void DL212_Config_Utility(void);  
unsigned char LRC( unsigned char *buf,unsigned short int len);
#endif

