#ifndef __MY_USB_H
#define __MY_USB_H

#include "usb_init.h"
#include "usb_pwr.h"
 
#define USB_RX_SIZE   99
#define USB_TX_SIZE   99

extern struct _USB_PARA sUSB_Para; 

struct _USB_PARA{
  unsigned int  rec_len;
	unsigned int  rec_ptr;
  unsigned char packet_sent;
	unsigned char packet_rec; 
  unsigned char rx_buf[USB_RX_SIZE];
  unsigned char	tx_buf[USB_TX_SIZE];
};
 
void My_USB_Init(void);
unsigned char USB_Send(unsigned char *buf,int len);
#endif
