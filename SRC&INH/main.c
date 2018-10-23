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
									BinarySemaphore_SDI12_FirstByte,\
                  BinarySemaphore_SDI12_CR_LF;									
SemaphoreHandle_t xSemaphore; 

TimerHandle_t OneShotTimer_Handle; 

int main(void){
	unsigned int i;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置系统中断优先级分组4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE); 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD,ENABLE); 
	delay_init(); //延时函数初始化 
	USART1_Config(115200);
	My_USB_Init(); 
	UserGpio_Config();  
	IIC_PCF_GPIO_Init();
	//PCF8563_Config(); 
	psFram_Func->init(); 
  psFlash_Func->init(); 
	psSDI12_Func->init(0);	
	psSDI12_Func->init(1); 
  psSW12_Func->init();//SW12控制口初始化
	psSW12_Func->sw(0,1);
	TIM5_TI1FP1_Init();//F_Mea---PA0(TIM5_CH1) 
  TIM2_TI2FP2_Init();//PSW-----PA1(TIM2_CH2)
	if(sDL212_Config.mea_time[0]){
	  TIM10_Init(sDL212_Config.mea_time[0]);
	}
	else{
	  TIM10_Init(1000);
	} 
	if(sDL212_Config.mea_time[1]){
	  TIM6_Init(sDL212_Config.mea_time[1]);
	}
	else{
	  TIM6_Init(1000);
	}   
	//delay_ms(3000); 
  //DBGMCU_Config(DBGMCU_SLEEP,ENABLE);
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
  xQueue = xQueueCreate(2,sizeof(char));
	BinarySemaphore_MB = xSemaphoreCreateBinary(); 
  BinarySemaphore_USB = xSemaphoreCreateBinary();
	BinarySemaphore_SDI12_FirstByte = xSemaphoreCreateBinary(); 
	BinarySemaphore_SDI12_CR_LF = xSemaphoreCreateBinary(); 
 	xSemaphore = xSemaphoreCreateMutex(); 
  if(xQueue==NULL || BinarySemaphore_MB==NULL || BinarySemaphore_USB==NULL     || 
		 BinarySemaphore_SDI12_FirstByte==NULL ||BinarySemaphore_SDI12_CR_LF==NULL ||
	   xSemaphore==NULL){
	  while(1); 
	} 
	
	ADS1248_SPI_Init(); 
	ADS1248_GPIO_Init();
	ADS1248SetVoltageReference(1); 	 
  ADS1248SetGain(0); 
	ADS1248SetDataRate(9);   

	//psC_Pulse_Func->init(0);//C1脉冲测量
	//psC_Pulse_Func->init(1);//C2脉冲测量   
 // sDL212_State.battery = psSE_FUNC->bat_read();
 // psSE_FUNC->vref_read();  
	
  //TIM3_ETR_Init();   //C2------PD2(TIM3_ETR)
  //TIM9_TI2FP2_Init();//C1------PA3(TIM9_CH2) 
	//SDI12_C1_SEND_DISABLE();
	//psC_RS232_Func->init(0,0);  
	/*OneShotTimer_Handle = xTimerCreate((const char*)"OneShotTimer",
		                                 (TickType_t)200,//200ms
	                                   (UBaseType_t)pdFALSE,
	                                   (void*)2,
															       (TimerCallbackFunction_t)OneShotCallback);*/
    xTaskCreate((TaskFunction_t )Task1, 
                (const char*    )"task for ...", 
                (uint16_t       )128, 
                (void*          )NULL, 
                (UBaseType_t    )2,	
                (TaskHandle_t*  )&Task1_Handler); 

    /*xTaskCreate((TaskFunction_t )Task2,
                (const char*    )"task for ...",   
                (uint16_t       )1024, 
                (void*          )NULL,
                (UBaseType_t    )1,
                (TaskHandle_t*  )&Task2_Handler);*/
	  /*xTaskCreate((TaskFunction_t )Task3,     
                (const char*    )"task for ...",   
                (uint16_t       )1024, 
                (void*          )NULL,
                (UBaseType_t    )1,
                (TaskHandle_t*  )&Task3_Handler); */
    vTaskDelete(Task_Start_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}
  
unsigned int DL212_EasyMode_Scan_Count=0; 
float volt[3];
void Task1(void *pvParameters){ 
	TickType_t xLastWakeTime; 
	const TickType_t xPeriod=pdMS_TO_TICKS(1000);  
	
	//DL212_EasyMode_Init(); 
	//xLastWakeTime = xTaskGetTickCount();  
  while(1){  
    //vTaskDelayUntil(&xLastWakeTime,xPeriod); 
		vTaskDelay(1000); 
		//volt[0] = VoltSE(0,0);
		//volt[1] = VoltSE(1,0);
		volt[2] = VoltDiff(0,0);  
		/*if(DL212_EasyMode){ 
      DL212_EasyMode_Scan(); 
      DL212_EasyMode_Scan_Count++; 
			memcpy(usRegInputBuf,Value,28); 
		} 
		else{ 
		  vTaskSuspend(Task1_Handler); 
		} */
	} 
} 

void Task2(void *pvParameters){ 
	BaseType_t err=pdFALSE; 
 
  while(1){ 
    if(USB_DETECT()){ 
			SDI12_Transparent(1);
      /*if(bDeviceState == CONFIGURED){ 
        CDC_Receive_DATA(); 
        if(sUSB_Para.packet_rec){				
					DL212_Config_Utility();
					//USB_Send(sUSB_Para.rx_buf,sUSB_Para.rec_len); 
          sUSB_Para.packet_rec = 0; 
          sUSB_Para.rec_len = 0; 
        } 
      }*/
	  } 
		else{ 
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
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
	
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