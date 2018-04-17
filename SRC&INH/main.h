#ifndef __MAIN_H
#define __MAIN_H
#include "FreeRTOS.h" 
#include "task.h"

extern TaskHandle_t Task_Start_Handler; 
extern TaskHandle_t Task_USB_Handler; 
extern TaskHandle_t Task_ADC_Handler; 
extern TaskHandle_t Task_DL212_EasyMode_Handler; 
 
void Task_Start(void *pvParameters); 
void Task_USB(void *pvParameters); 
void Task_ADC(void *pvParameters);
void Task_ADC(void *pvParameters);
void Task_DL212_EasyMode(void *pvParameters);

void UserGpio_Config(void);
#endif
