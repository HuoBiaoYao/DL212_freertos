#ifndef __HW_C_PORT_H
#define __HW_C_PORT_H


#define C1_CTRL_PORT     GPIOB
#define C1_CTRL_PIN      GPIO_Pin_7
#define C1_TX_PORT       GPIOA
#define C1_TX_PIN        GPIO_Pin_2
#define C1_RX_PORT       GPIOA
#define C1_RX_PIN        GPIO_Pin_3
#define C1_RX_PIN_SOURCE GPIO_PinSource3 

#define C2_CTRL_PORT     GPIOB
#define C2_CTRL_PIN      GPIO_Pin_6
#define C2_TX_PORT       GPIOC
#define C2_TX_PIN        GPIO_Pin_12 
#define C2_RX_PORT       GPIOD
#define C2_RX_PIN        GPIO_Pin_2 
#define C2_RX_PIN_SOURCE GPIO_PinSource2

typedef enum __C_PORT_STATE{ 
	C_SDI12_IN_USE=0,
	C_RS232_TX_IN_USE,
	C_RS232_RX_IN_USE,
	C_OUT_IN_USE,
	C_PULSE_IN_USE, 
}_C_PORT_STATE;

extern _C_PORT_STATE eC_PORT_STATE[2];
 
#endif
