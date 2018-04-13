#include "main.h" 
#include "hw.h"
#include "delay.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "my_usb.h"
#include "DL212_easy_mode.h"
  
TaskHandle_t Task_Start_Handler; 
TaskHandle_t Task_USB_Handler; 
TaskHandle_t Task_ADC_Handler; 
void Task_Start(void *pvParameters); 
void Task_USB(void *pvParameters); 
void Task_ADC(void *pvParameters);

 
//二值信号量句柄
SemaphoreHandle_t BinarySemaphore; 
 
int main(void){ 
	unsigned int i; 
 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE); 
	delay_init();               //延时函数初始化	  
	uart_init(115200);					//初始化串口
	SW12_Port_Init(); 
  UserGpio_Config();	
	SE_ADC_Init(); 
	//My_USB_Init(); 
	delay_ms(2000);
	//创建开始任务
  xTaskCreate((TaskFunction_t )Task_Start,           //任务函数
              (const char*    )"start_task",         //任务名称
              (uint16_t       )32,                   //任务堆栈大小
              (void*          )NULL,                 //传递给任务函数的参数
              (UBaseType_t    )1,                    //任务优先级
              (TaskHandle_t*  )&Task_Start_Handler); //任务句柄              
  vTaskStartScheduler();          //开启任务调度

  return 0;
} 

//开始任务任务函数
void Task_Start(void *pvParameters){
    taskENTER_CRITICAL();           //进入临界区

    xTaskCreate((TaskFunction_t )Task_USB,     	
                (const char*    )"task for usb",   	
                (uint16_t       )32, 
                (void*          )NULL,				
                (UBaseType_t    )1,	
                (TaskHandle_t*  )&Task_USB_Handler);   

    xTaskCreate((TaskFunction_t )Task_ADC,     
                (const char*    )"task for adc",   
                (uint16_t       )32, 
                (void*          )NULL,
                (UBaseType_t    )1,
                (TaskHandle_t*  )&Task_ADC_Handler); 
    vTaskDelete(Task_Start_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}
 
unsigned int Task_USB_Count=0; 
void Task_USB(void *pvParameters){ 
  while(1){ 
		vTaskDelay(1000); 
		Task_USB_Count++; 
		/*if(USB_DETECT()){ 
	    if(bDeviceState == CONFIGURED){ 
  	    CDC_Receive_DATA(); 
			  if(sUSB_Para.packet_rec){ 
					USB_Send(sUSB_Para.rx_buf,sUSB_Para.rec_len);
			  	sUSB_Para.packet_rec = 0; 
				  sUSB_Para.rec_len = 0; 
			  } 
	    } 
	  }*/
  }
}   
 float adc_value;
unsigned int Task_ADC_Count=0;
void Task_ADC(void *pvParameters){
	portTickType xLastWakeTime; 
	
  while(1){ 
		adc_value = SE_AdcValue_Read(0);
		vTaskDelay(1000); 
		//vTaskDelayUntil(&xLastWakeTime,(500/portTICK_RATE_MS));
		Task_ADC_Count++;
	}
}

void UserGpio_Config(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC,ENABLE); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIOB->BSRRL = GPIO_Pin_5;
 
	//2.5v and ref
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	//se1-se4
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
  