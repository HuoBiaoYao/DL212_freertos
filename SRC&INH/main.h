#ifndef __MAIN_H
#define __MAIN_H
#include "FreeRTOS.h"  
#include "semphr.h"
#include "task.h"
#include "timers.h"

extern TaskHandle_t Task_Start_Handler; 
extern TaskHandle_t Task1_Handler; 
extern TaskHandle_t Task2_Handler; 
extern TaskHandle_t Task3_Handler; 
extern TaskHandle_t Task4_Handler; 

extern QueueHandle_t xQueue; 
extern SemaphoreHandle_t BinarySemaphore_Modbus;
extern SemaphoreHandle_t BinarySemaphore_USB;   
extern SemaphoreHandle_t xSemaphore;
extern unsigned char MB_Ctrl;
extern TimerHandle_t OneShotTimer_Handle;

void Task_Start(void *pvParameters); 
void Task1(void *pvParameters); 
void Task2(void *pvParameters);
void Task3(void *pvParameters); 
void Task4(void *pvParameters);

void OneShotCallback(TimerHandle_t xTimer);
void UserGpio_Config(void);
#endif
