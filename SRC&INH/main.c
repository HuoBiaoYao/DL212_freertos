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
#include "mb.h" 
#include "mbrtu.h"
#include "portfunction.h" 
 
TaskHandle_t Task_Start_Handler; 
TaskHandle_t Task1_Handler; 
TaskHandle_t Task2_Handler; 
TaskHandle_t Task3_Handler; 
TaskHandle_t Task4_Handler; 
 
QueueHandle_t xQueue; 
SemaphoreHandle_t BinarySemaphore_Modbus; 
SemaphoreHandle_t BinarySemaphore_USB;  
SemaphoreHandle_t xSemaphore; 

TimerHandle_t OneShotTimer_Handle; 

char InfoBuffer[1000]; 
unsigned char MB_Ctrl=0;
int main(void){ 
	unsigned int i; 
 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����ϵͳ�ж����ȼ�����4	 	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE); 
	delay_init(); //��ʱ������ʼ�� 
	delay_ms(3000); 
  //DBGMCU_Config(DBGMCU_SLEEP,ENABLE);
	//������ʼ����
  xTaskCreate((TaskFunction_t )Task_Start,           //������
              (const char*    )"start_task",         //��������
              (uint16_t       )128,                   //�����ջ��С
              (void*          )NULL,                 //���ݸ��������Ĳ���
              (UBaseType_t    )1,                    //�������ȼ�
              (TaskHandle_t*  )&Task_Start_Handler); //������              
  vTaskStartScheduler();          //�����������

  return 0;
} 

//��ʼ����������
void Task_Start(void *pvParameters){
  taskENTER_CRITICAL();           //�����ٽ���
  xQueue = xQueueCreate(10,sizeof(char));
  BinarySemaphore_USB = xSemaphoreCreateBinary(); 
	BinarySemaphore_Modbus = xSemaphoreCreateBinary(); 
 	xSemaphore = xSemaphoreCreateMutex(); 
  if(xQueue==NULL || BinarySemaphore_USB==NULL || xSemaphore==NULL){
	  while(1);
	}
	My_USB_Init(); 
	psSE_FUNC->init(); 
	psFram_Func->init(); 
  psFlash_Func->init();  
	psSW12_Func->init();//SW12���ƿڳ�ʼ��
	psC_Pulse_Func->init(0);//C1��Ĭ�ϳ�ʼ��Ϊ�������
	psC_Pulse_Func->init(1);//C2��Ĭ�ϳ�ʼ��Ϊ�������  
 // sDL212_State.battery = psSE_FUNC->bat_read();
 // psSE_FUNC->vref_read(); 
  I2C_RTC_Init();  
  //TIM2_TI2FP2_Init();//PSW-----PA1(TIM2_CH2)
  TIM3_ETR_Init();   //C2------PD2(TIM3_ETR)
  TIM9_TI2FP2_Init();//C1------PA3(TIM9_CH2) 
	TIM5_TI1FP1_Init();//F_Mea---PA0(TIM5_CH1)  
	/*SDI12_C1_SEND_DISABLE();
	psSDI12_Func->init(0);
	psSDI12_Func->init(1);
	psC_RS232_Func->init(0,0);*/  
	OneShotTimer_Handle = xTimerCreate((const char*)"OneShotTimer",
		                                 (TickType_t)200,//200ms
	                                   (UBaseType_t)pdFALSE,
	                                   (void*)0,
															       (TimerCallbackFunction_t)OneShotCallback);
	
    xTaskCreate((TaskFunction_t )Task1, 
                (const char*    )"usb communication", 
                (uint16_t       )128, 
                (void*          )NULL, 
                (UBaseType_t    )1,	
                (TaskHandle_t*  )&Task1_Handler);   

    xTaskCreate((TaskFunction_t )Task2,     
                (const char*    )"usart1 modbus",   
                (uint16_t       )512, 
                (void*          )NULL,
                (UBaseType_t    )1,
                (TaskHandle_t*  )&Task2_Handler); 
	  xTaskCreate((TaskFunction_t )Task3,     
                (const char*    )"DL212 easy mode scan",   
                (uint16_t       )256, 
                (void*          )NULL,
                (UBaseType_t    )1,
                (TaskHandle_t*  )&Task3_Handler); 
		xTaskCreate((TaskFunction_t )Task4,     
                (const char*    )"get task infomation ",   
                (uint16_t       )512, 
                (void*          )NULL,
                (UBaseType_t    )1,
                (TaskHandle_t*  )&Task4_Handler); 						
    vTaskDelete(Task_Start_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


void Task1(void *pvParameters){ 
	BaseType_t err=pdFALSE;
 
  while(1){
    if(USB_DETECT()){ 
      if(bDeviceState == CONFIGURED){ 
        CDC_Receive_DATA(); 
        if(sUSB_Para.packet_rec){ 
          DL212_EasyMode_Config(); 
          //USB_Send(sUSB_Para.rx_buf,sUSB_Para.rec_len); 
          sUSB_Para.packet_rec = 0; 
          sUSB_Para.rec_len = 0; 
        } 
      } 
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

void Task2(void *pvParameters){
	unsigned int i;
	portTickType xLastWakeTime;  
	
	for( i = 0; i < REG_HOLDING_NREGS; i++ ){
    usRegHoldingBuf[i] = ( unsigned short )i;
  }
  for( i = 0; i < REG_INPUT_NREGS; i++ ){
    usRegInputBuf[i] = ( unsigned short )i;
  }
  eMBInit( MB_RTU, 0x01, 0, 115200, MB_PAR_NONE ); 
  /* Enable the Modbus Protocol Stack. */
  eMBEnable();
	while(1){ 
		/*if(BinarySemaphore_Modbus != NULL){ 
      if(xSemaphoreTake(BinarySemaphore_Modbus,portMAX_DELAY) == pdTRUE){
        eMBPoll();  	  
			}
		}*/
		if(1 == MB_Ctrl){
			xTimerStart(OneShotTimer_Handle,0);
		  eMBPoll();
		}
	  vTaskDelay(10);
	}
} 
 
unsigned int DL212_EasyMode_Scan_Count=0; 
void Task3(void *pvParameters){ 
	TickType_t xLastWakeTime; 
	const TickType_t xPeriod=pdMS_TO_TICKS(1000); 
	
	//DL212_EasyMode_Init();
	xLastWakeTime = xTaskGetTickCount(); 
  while(1){ 
    //vTaskDelay(10);
		vTaskDelayUntil(&xLastWakeTime,xPeriod); 
		if(DL212_EasyMode){ 
      //DL212_EasyMode_Scan(); 
      DL212_EasyMode_Scan_Count++; 
		} 
		else{ 
		  vTaskSuspend(Task3_Handler); 
		} 
	} 
} 
 
void Task4(void *pvParameters){ 
  unsigned int TotalRunTime;
	UBaseType_t ArraySize,x;
	TaskStatus_t *StatusArray;
	
	//��һ��������uxTaskGetSystemState()��ʹ��
	printf("/********��һ��������uxTaskGetSystemState()��ʹ��**********/\r\n");
	ArraySize=uxTaskGetNumberOfTasks();		//��ȡϵͳ��������
	StatusArray=pvPortMalloc(ArraySize*sizeof(TaskStatus_t));//�����ڴ�
	if(StatusArray!=NULL){ //�ڴ�����ɹ�
		ArraySize=uxTaskGetSystemState((TaskStatus_t *)StatusArray, 	//������Ϣ�洢����
									                 (UBaseType_t)ArraySize, 	//������Ϣ�洢�����С
								                   (unsigned int *)&TotalRunTime);//����ϵͳ�ܵ�����ʱ��
		printf("TaskName\t\tPriority\t\tTaskNumber\t\t\r\n");
		for(x=0;x<ArraySize;x++){
			//ͨ�����ڴ�ӡ����ȡ����ϵͳ������й���Ϣ�������������ơ�
			//�������ȼ��������š�
			printf("%s\t\t%d\t\t\t%d\t\t\t\r\n",				
					StatusArray[x].pcTaskName,
					(int)StatusArray[x].uxCurrentPriority,
					(int)StatusArray[x].xTaskNumber);
		}
	}
	vPortFree(StatusArray);	//�ͷ��ڴ�
	printf("/**************************����***************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	//while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);		//�ȴ�KEY_UP������
	//�ڶ���������vTaskGetInfo()��ʹ��
	TaskHandle_t TaskHandle;	
	TaskStatus_t TaskStatus;
	
	printf("/************�ڶ���������vTaskGetInfo()��ʹ��**************/\r\n");
	TaskHandle=xTaskGetHandle("led0_task");			//������������ȡ��������
	//��ȡLED0_Task��������Ϣ
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 		//������
				 (TaskStatus_t*	)&TaskStatus, 		//������Ϣ�ṹ��
				 (BaseType_t	)pdTRUE,			//����ͳ�������ջ��ʷ��Сʣ���С
			     (eTaskState	)eInvalid);			//�����Լ���ȡ��������׳̬
	//ͨ�����ڴ�ӡ��ָ��������й���Ϣ��
	printf("������:                %s\r\n",TaskStatus.pcTaskName);
	printf("������:              %d\r\n",(int)TaskStatus.xTaskNumber);
	printf("����׳̬:              %d\r\n",TaskStatus.eCurrentState);
	printf("����ǰ���ȼ�:        %d\r\n",(int)TaskStatus.uxCurrentPriority);
	printf("��������ȼ�:          %d\r\n",(int)TaskStatus.uxBasePriority);
	printf("�����ջ����ַ:        %#x\r\n",(int)TaskStatus.pxStackBase);
	printf("�����ջ��ʷʣ����Сֵ:%d\r\n",TaskStatus.usStackHighWaterMark);
	printf("/**************************����***************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	//while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);		//�ȴ�KEY_UP������
	
	//������������eTaskGetState()��ʹ��	
	eTaskState TaskState;
	char TaskInfo[10];
	printf("/***********������������eTaskGetState()��ʹ��*************/\r\n");
	TaskHandle=xTaskGetHandle("query_task");		//������������ȡ��������
	TaskState=eTaskGetState(TaskHandle);			//��ȡquery_task���������׳̬
	memset(TaskInfo,0,10);						
	switch((int)TaskState)
	{
		case 0:
			sprintf(TaskInfo,"Running");
			break;
		case 1:
			sprintf(TaskInfo,"Ready");
			break;
		case 2:
			sprintf(TaskInfo,"Suspend");
			break;
		case 3:
			sprintf(TaskInfo,"Delete");
			break;
		case 4:
			sprintf(TaskInfo,"Invalid");
			break;
	}
	printf("����׳ֵ̬:%d,��Ӧ��׳̬Ϊ:%s\r\n",TaskState,TaskInfo);
	printf("/**************************����**************************/\r\n");
	printf("����KEY_UP������!\r\n\r\n\r\n");
	//while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);		//�ȴ�KEY_UP������
	
	//���Ĳ�������vTaskList()��ʹ��	
	printf("/*************������������vTaskList()��ʹ��*************/\r\n");
	vTaskList(InfoBuffer);							//��ȡ�����������Ϣ
	printf("%s\r\n",InfoBuffer);					//ͨ�����ڴ�ӡ�����������Ϣ
	printf("/**************************����**************************/\r\n");
	while(1)
	{
		//LED1=~LED1;
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}

int idlehookcnt=0; 
void vApplicationIdleHook(void){
  idlehookcnt++;  
	__disable_irq();
	__dsb(portSY_FULL_READ_WRITE );
	__isb(portSY_FULL_READ_WRITE );
	
	//BeforeEnterSleep();		//����˯��ģʽ֮ǰ��Ҫ���������
	SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);
	__wfi();				//����˯��ģʽ
	//PWR_EnterSleepMode(PWR_Regulator_LowPower,PWR_SLEEPEntry_WFI); 
	//AfterExitSleep();		//�˳�˯��ģʽ֮����Ҫ��������� 
	__dsb(portSY_FULL_READ_WRITE );
	__isb(portSY_FULL_READ_WRITE );
	__enable_irq();
}

int tickhookcnt=0; 
void vApplicationTickHook(void){
  tickhookcnt++; 
}

void OneShotCallback(TimerHandle_t xTimer){ 
	BaseType_t xHigherPriorityTaskWoken, xResult;
	
	MB_Ctrl = 0;
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
  