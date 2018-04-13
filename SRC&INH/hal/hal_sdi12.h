#ifndef  __HAL_SDI12_H
#define  __HAL_SDI12_H
#include "hw_c_port.h"

#define SDI12_RX_SIZE   80
#define SDI12_TX_SIZE   80
  
#define SDI12_C1_SEND_ENABLE()    C1_CTRL_PORT->BSRRL = C1_CTRL_PIN;//GPIO_SetBits  (C1_CTRL_PORT,C1_CTRL_PIN) 
#define SDI12_C1_SEND_DISABLE()   C1_CTRL_PORT->BSRRH = C1_CTRL_PIN;//GPIO_ResetBits(C1_CTRL_PORT,C1_CTRL_PIN)
#define SDI12_C2_SEND_ENABLE()    C2_CTRL_PORT->BSRRL = C2_CTRL_PIN;GPIO_SetBits  (C1_CTRL_PORT,C2_CTRL_PIN)
#define SDI12_C2_SEND_DISABLE()   C2_CTRL_PORT->BSRRH = C2_CTRL_PIN;GPIO_ResetBits(C1_CTRL_PORT,C2_CTRL_PIN)

#define SDI12_C1_HIGH()           C1_TX_PORT->BSRRL = C1_TX_PIN;//GPIO_SetBits  (C1_TX_PORT,C1_TX_PIN)  
#define SDI12_C1_LOW()            C1_TX_PORT->BSRRH = C1_TX_PIN;//GPIO_ResetBits(C1_TX_PORT,C1_TX_PIN)
#define SDI12_C2_HIGH()           C2_TX_PORT->BSRRL = C2_TX_PIN;//GPIO_SetBits  (C2_TX_PORT,C2_TX_PIN)  
#define SDI12_C2_LOW()            C2_TX_PORT->BSRRH = C2_TX_PIN;//GPIO_ResetBits(C2_TX_PORT,C2_TX_PIN)

#define SDI12_C1_START()          SDI12_C1_HIGH()
#define SDI12_C1_STOP()           SDI12_C1_LOW()
#define SDI12_C2_START()          SDI12_C2_HIGH()
#define SDI12_C2_STOP()           SDI12_C2_LOW()

  
typedef enum{
	SDI12_OK = 0,		 
	SDI12_NODATA,		 
	SDI12_TIMEOUT, 
	SDI12_CHECKERR, 
	SDI12_SBITERR, 
	SDI12_IBITERR,		
	SDI12_NOACK, 
	SDI12_NOT_IN_USE
}__SDI12_RSL;

 
typedef enum{ 
	SDI12_DISABLE,
	SDI12_IDLE,
	SDI12_BUSY,
	SDI12_SEND,
	SDI12_REICEIVE, 
}__SDI12_BUS;

typedef enum{
	SDI12CMD_TEST = 0, 
	SDI12CMD_INFO, 
	SDI12CMD_MEAS, 
}__SDI12_COMMAND;  

struct _SDI12_FUNC{
	void (*init)(unsigned char port);
  __SDI12_RSL (*transparent)(unsigned char port);
	__SDI12_RSL (*read)(unsigned char port,unsigned char *dst,unsigned char *src);
	__SDI12_RSL (*send)(unsigned char port,unsigned char *string,unsigned int len);
	__SDI12_RSL (*sendbyte)(unsigned char port,unsigned char *byte);
	void (*send_enable)(unsigned char port);
	void (*send_disable)(unsigned char port); 
	void (*bus_high)(unsigned char port);
	void (*bus_low) (unsigned char port);
	void (*start)   (unsigned char port);
	void (*stop)    (unsigned char port);
};

struct _SDI12_PARA{
  unsigned char tx_buf[SDI12_TX_SIZE];
	unsigned char rx_buf[SDI12_RX_SIZE];
	unsigned int lenth;
	unsigned int rx_ptr;
	unsigned int tx_ptr;
};


void SDI12_Init(unsigned char port);
void SDI12_Send_Enable(unsigned char port);
void SDI12_Send_Disable(unsigned char port);
void SDI12_High(unsigned char port);
void SDI12_Low(unsigned char port);
void SDI12_Start(unsigned char port);
void SDI12_Stop(unsigned char port);
__SDI12_RSL SDI12_Read(unsigned char port,unsigned char *dst,unsigned char *src);
__SDI12_RSL SDI12_Send(unsigned char port,unsigned char *string,unsigned int len); 
__SDI12_RSL SDI12_SendByte(unsigned char port,unsigned char *byte);
__SDI12_RSL SDI12_Transparent(unsigned char port);
 
   
extern __SDI12_BUS eSDI12_BUS[2];
extern struct _SDI12_PARA sSDI12_Para[2];
extern struct _SDI12_FUNC sSDI12_Func;
extern struct _SDI12_FUNC *psSDI12_Func;
#endif
