#ifndef __DATALOGGER_PROTOCOLS_H
#define __DATALOGGER_PROTOCOLS_H
#include "time.h"


#define PROTOCOL_BUF_LEN 550 
#define PACK_LEN_MAX 1056

#define SERSYNCBYTE 0xAE   //当传输数据中出现0xAE时变成0xAC 0xCE
#define QUOTEBYTE   0xAC   //当传输数据中出现0xAC时变成0xAC 0xCC
#define RBYTE_0     0xCE
#define RBYTE_1     0xCC

struct _DATALOGGER_TIME{
  unsigned int time_1970;
  unsigned char rtc[8];
  struct tm stime;
};
extern struct _DATALOGGER_TIME sDatalogger_Time;

struct _PROTOCOL_BUF{
  unsigned char tx_buf[PROTOCOL_BUF_LEN]; 
	unsigned char rx_buf[PROTOCOL_BUF_LEN]; 
	unsigned int  rx_len; 
	unsigned int  tx_len; 
};
extern struct _PROTOCOL_BUF sPBuf;

struct _DATALOGGER_INFO{
  unsigned char type[24]; 
	unsigned char os_version[24]; 
	unsigned char protocol_version[24]; 
};
extern struct _DATALOGGER_INFO sDL212_Info;

struct _DATALOGGER_STATE{
  float battery;
	float lithium_battery;
	float temperature;
	unsigned int wd_erro_cnt;
	unsigned int skip_scan;
	unsigned int skip_record;
	unsigned int sd_mem_total;
	unsigned int sd_mem_avlbl;
	unsigned int sd_time_total;
	unsigned int sd_time_avlbl;
};
extern struct _DATALOGGER_STATE sDL212_State;

struct _PROGRAM_INFO{
  unsigned char name[50];
	unsigned int lenth;
	unsigned short int crc;
	unsigned int time;
	unsigned int offset;
}; 
 extern struct _PROGRAM_INFO sProgram_Info;
  
struct _TABLE_INFO{ 
  unsigned int time_intoint; 
	unsigned int time_interval;
	unsigned int time_unit;
	unsigned int record_len; 
	unsigned int record_count;
	unsigned int record_min; 
  unsigned int record_max; 
	unsigned short int crc;
}; 
extern struct _TABLE_INFO sTable_Info[];
  
struct _PROTOCOL_FUNCTION{
	char (*packet_respond)(void);
  char (*packet_send)(unsigned int len);
	char (*erase_data)(void);
	char (*delete_program)(void);
	char (*get_record)(unsigned char *buf,unsigned int beg_record,unsigned int num_of_record);
	char (*get_program_info)(void);
	char (*get_table_definition)(void);
	char (*download_program)(unsigned char *buf,unsigned int offset,unsigned int len);
	unsigned short int (*program_crc_calculate)(unsigned int len);
	unsigned short int (*table_crc_calculate)(unsigned int len);
	time_t (*set_datalogger_time)(time_t time);
};
extern struct _PROTOCOL_FUNCTION sProtocol_Func;
 
char Packet_Respond(void); 
unsigned int SendPb(unsigned char *buf,unsigned int lenth); 
unsigned int ReadPb(unsigned char *buf,unsigned int lenth); 
unsigned short int CRC16(unsigned char *buf,unsigned short int len);
 

enum _MSG_TYPE{ 
	CONNECT_CMD           = 0x00,
	CONNECT_RSP           = 0xFF,
  DISCONNECT_CMD        = 0x01,
	DISCONNECT_RSP        = 0xFE,
	CLOCK_CMD             = 0x02,
	CLOCK_RSP             = 0xFD,
	GETINFO_CMD           = 0x03,
  GETINFO_RSP           = 0xFC,
  GETSTATE_CMD          = 0x04,
  GETSTATE_RSP          = 0xFB,
	GETDATA_CMD           = 0x05,
	GETDATA_RSP           = 0xFA,
	ERASEDATA_CMD         = 0x06,
	ERASEDATA_RSP         = 0xF9,
	DELETEPROGRAM_CMD     = 0x07,
	DELETEPROGRAM_RSP     = 0xF8,
	DOWNLOADPROGRAM_CMD   = 0x08, 
	DOWNLOADPROGRAM_RSP   = 0xF7,
	GETPROGRAMINFO_CMD    = 0x09,
	GETPROGRAMINFO_RSP    = 0xF6,
	GETTABLEDEFINITION_CMD= 0x0A,
	GETTABLEDEFINITION_RSP= 0xF5,
	SETPUBLIC_CMD         = 0x0B,  
	SETPUBLIC_RSP         = 0xF4,
	WAIT_CMD              = 0x20 
};
 


#endif
