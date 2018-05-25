#ifndef _PORTFUNCTION_H
#define _PORTFUNCTION_H

/* STM32 Includes ------------------------------------------------------------*/
#include "stm32l1xx.h"

/* FreeRTOS Includes ---------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Modbus Includes -----------------------------------------------------------*/
#include "mb.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define REG_INPUT_START     0x0001
#define REG_INPUT_NREGS     14
#define REG_HOLDING_START 	0x0001
#define REG_HOLDING_NREGS 	1

/* Private variables ---------------------------------------------------------*/
extern uint16_t usRegInputStart;
extern uint16_t usRegInputBuf[REG_INPUT_NREGS];
extern uint16_t usRegHoldingStart;
extern uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];


#endif