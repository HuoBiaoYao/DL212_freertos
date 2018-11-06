#include "main.h" 
#include "hw.h"
#include "hal.h"
#include "delay.h" 
#include "task.h"
#include "semphr.h"
#include "my_usb.h"
#include "DL212_easy_mode.h"
 #include "issort.h" 
 #include "math.h"
/* Modbus Includes -----------------------------------------------------------*/
#include "usart_idle_dma_modbus.h"   
#include "portfunction.h"
//new
#include "ads1248.h" 
#include "iic.h"
#include "pcf8563.h"
#include "mcp4725.h"

TaskHandle_t Task_Start_Handler; 
TaskHandle_t Task1_Handler; 
TaskHandle_t Task2_Handler; 
TaskHandle_t Task3_Handler; 
 
QueueHandle_t xQueue;
SemaphoreHandle_t BinarySemaphore_MB,\
                  BinarySemaphore_USB,\
									BinarySemaphore_USART,\
									BinarySemaphore_SDI12_FirstByte,\
                  BinarySemaphore_SDI12_CR_LF;									
SemaphoreHandle_t xSemaphore; 

TimerHandle_t OneShotTimer_Handle; 

int main(void){
	unsigned int i;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置系统中断优先级分组4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD,ENABLE);  
	EXTI_DeInit();
	delay_init(); //延时函数初始化 
	USART1_Config(115200);
	My_USB_Init(); 
	UserGpio_Config(); 
	psFram_Func->init(); 
  psFlash_Func->init();
	IIC_PCF_GPIO_Init(); 
	psSW12_Func->init();//SW12控制口初始化
	ADS1248_Init(); 
	DL212_EasyMode_Init();  
	//delay_ms(3000); 
  //DBGMCU_Config(DBGMCU_SLEEP,ENABLE);
	//创建开始任务
  xTaskCreate((TaskFunction_t )Task_Start,           //任务函数
              (const char*    )"start_task",         //任务名称
              (uint16_t       )64,                   //任务堆栈大小
              (void*          )NULL,                 //传递给任务函数的参数
              (UBaseType_t    )1,                    //任务优先级
              (TaskHandle_t*  )&Task_Start_Handler); //任务句柄              
  vTaskStartScheduler();          //开启任务调度

  return 0;
} 

//开始任务任务函数
void Task_Start(void *pvParameters){
  taskENTER_CRITICAL();           //进入临界区
  xQueue = xQueueCreate(2,sizeof(char));
	BinarySemaphore_MB = xSemaphoreCreateBinary(); 
  BinarySemaphore_USB = xSemaphoreCreateBinary();
	BinarySemaphore_USART = xSemaphoreCreateBinary();
	BinarySemaphore_SDI12_FirstByte = xSemaphoreCreateBinary(); 
	BinarySemaphore_SDI12_CR_LF = xSemaphoreCreateBinary(); 
 	xSemaphore = xSemaphoreCreateMutex(); 
  if(xQueue==NULL || BinarySemaphore_MB==NULL || BinarySemaphore_USB==NULL || BinarySemaphore_USART==NULL || BinarySemaphore_SDI12_FirstByte==NULL ||BinarySemaphore_SDI12_CR_LF==NULL || xSemaphore==NULL){
	  delay_ms(10000);  
	} 
	//SDI12_C1_SEND_DISABLE();
	//psC_RS232_Func->init(0,0);  
	/*OneShotTimer_Handle = xTimerCreate((const char*)"OneShotTimer",
		                                 (TickType_t)200,//200ms
	                                   (UBaseType_t)pdFALSE,
	                                   (void*)2,
															       (TimerCallbackFunction_t)OneShotCallback);*/
    xTaskCreate((TaskFunction_t )Task1, 
                (const char*    )"task for ...", 
                (uint16_t       )512, 
                (void*          )NULL, 
                (UBaseType_t    )2,	
                (TaskHandle_t*  )&Task1_Handler); 
    xTaskCreate((TaskFunction_t )Task2,
                (const char*    )"task for ...",   
                (uint16_t       )512, 
                (void*          )NULL,
                (UBaseType_t    )1,
                (TaskHandle_t*  )&Task2_Handler);
    vTaskDelete(Task_Start_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}
  
unsigned int Scan_Count=0; 
void Task1(void *pvParameters){ 
	TickType_t xLastWakeTime; 
	const TickType_t xPeriod=pdMS_TO_TICKS(1000);  
	 
	xLastWakeTime = xTaskGetTickCount();
	LED_SCAN_ON();
	vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(500));LED_SCAN_OFF(); 
	vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(100));LED_SCAN_ON(); 
	vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(100));LED_SCAN_OFF(); 
	vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(100));LED_SCAN_ON(); 
	vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(100));LED_SCAN_OFF(); 
	vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(100));LED_SCAN_ON(); 
	vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(100));LED_SCAN_OFF(); 
  while(1){  
		if(SDI12_0_TRANSPARENT!=DL212_DebugMode && SDI12_1_TRANSPARENT!=DL212_DebugMode){
			LED_SCAN_ON(); 
      DL212_EasyMode_Scan(); 
		}
		Scan_Count++; 
		vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(30));
    LED_SCAN_OFF();	
		#ifndef USE_PCF8563
		if(sDL212_Config.scan){
		  vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(sDL212_Config.scan)-20);
		}
		else{
		  vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(980));
		}
		#else
		vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(980));
		#endif		
		if(SDI12_0_TRANSPARENT==DL212_DebugMode || SDI12_1_TRANSPARENT==DL212_DebugMode){
			vTaskSuspend(Task1_Handler); 
		}
	} 
} 

void Task2(void *pvParameters){ 
	BaseType_t err=pdFALSE; 
  static unsigned int count=0;
	
  while(1){ 
    if(USB_DETECT()){
			if(bDeviceState == CONFIGURED){ 
				CDC_Receive_DATA(); 
				if(sUSB_Para.packet_rec){				
					DL212_Config_Utility();
					//USB_Send(sUSB_Para.rx_buf,sUSB_Para.rec_len);  
					switch(DL212_DebugMode){
						case SDI12_0_TRANSPARENT:
							SDI12_Transparent(0);
						break;
						case SDI12_1_TRANSPARENT:
							SDI12_Transparent(1);
						break;
						default:
			      break; 
					  sUSB_Para.packet_rec = sUSB_Para.rec_len  = 0;	
					}
				} 
				if(Scan_Count > count){
				  count = Scan_Count;
					if(VALUE_DISPLAY_ON == DL212_DebugMode){
						USB_Send(sUSB_Para.tx_buf,sUSB_Para.tx_len); 
						memset(sUSB_Para.tx_buf,0,400);
						sUSB_Para.tx_len = 0;
					}
				}				
			}
			if(VALUE_DISPLAY_ON==DL212_DebugMode || VALUE_DISPLAY_OFF==DL212_DebugMode){
				vTaskResume(Task1_Handler); 
			}
	  } 
		else{ 
			if(SDI12_0_TRANSPARENT==DL212_DebugMode || SDI12_1_TRANSPARENT==DL212_DebugMode){
				DL212_DebugMode = VALUE_DISPLAY_ON;
				vTaskResume(Task1_Handler); 
			}
		  if(BinarySemaphore_USB != NULL){ 
        if(xSemaphoreTake(BinarySemaphore_USB,portMAX_DELAY) == pdTRUE){ 
	        SystemInit(); 
				} 
			} 
		} 
  } 
} 
 
void Task3(void *pvParameters){
	portTickType xLastWakeTime; 
  unsigned char i=0;
	
	sMBSlave.init(115200); 
	while(1){ 
	  if(BinarySemaphore_MB != NULL){ 
      /*if(xSemaphoreTake(BinarySemaphore_MB,portMAX_DELAY) == pdTRUE){ 
				if(1 == sDL212_Config.mode){
					ucMBAddress = sDL212_Config.modbus_addr;
				  sMBSlave.poll();
				}
        else{
				  memset(USART_DMA_RxBuf,0,USART1_DMA_Rec_Cnt);
				  USART1_DMA_Rec_Cnt =0;
				}
	    }*/
    }
	}
} 
 

int idlehookcnt=0; 
void vApplicationIdleHook(void){
  idlehookcnt++; 
}

int tickhookcnt=0; 
void vApplicationTickHook(void){
  tickhookcnt++; 
}

void OneShotCallback(TimerHandle_t xTimer){ 
	BaseType_t xHigherPriorityTaskWoken, xResult;
	
} 
 
void UserGpio_Config(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIOC->BSRRL = GPIO_Pin_7; 
}
  
void PCF8563_Config(void){
	_PCF8563_Time_Typedef sTime1={18,06,30},sTime2={0,0,0,0};
	_PCF8563_Date_Typedef sDate1={18,8,31,4},sDate2={0,0,0,0};
	unsigned char century;
	
	_PCF8563_CLKOUT_Typedef sPCF8563_CLKOUT={PCF_CLKOUT_F1,PCF_CLKOUT_Close};
 
	PCF8563_SetPulseOut(1); 
	PCF8563_SetCLKOUT(&sPCF8563_CLKOUT); 
  PCF8563_SetTime(PCF_Format_BIN,&sTime1); 
	PCF8563_SetDate(PCF_Format_BIN,PCF_Century_20xx,&sDate1); 
	PCF8563_GetTime(PCF_Format_BIN,&sTime2); 
	PCF8563_GetDate(PCF_Format_BIN,&century,&sDate2); 
}