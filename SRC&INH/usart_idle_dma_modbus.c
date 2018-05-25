#include "usart_idle_dma_modbus.h"
#include "hw_serial.h"
#include "main.h" 
#include "mbcrc.h"	 
	 
unsigned char MB_Address=1;

struct MB_FUNCTION sMBSlave={
  USART1_Config,
	MB_Slave_Poll,
	MB_Slave_Send
};
 
 
void MB_Slave_Poll(void){
	static UCHAR   *ucMBFrame;
  static UCHAR    ucRcvAddress;
  static UCHAR    ucFunctionCode;
  static USHORT   usLength;
  static eMBException eException;
	int             i; 
	 
  if((USART1_DMA_Rec_Cnt>=4) && (usMBCRC16((UCHAR *)USART_DMA_RxBuf,USART1_DMA_Rec_Cnt )==0)){
	  memcpy((void*)ucRTUBuf,USART_DMA_RxBuf,USART1_DMA_Rec_Cnt);	 
	  usLength = USART1_DMA_Rec_Cnt-3;
	  ucMBFrame = (unsigned char*)ucRTUBuf+1; 
	  ucRcvAddress   = *(ucRTUBuf);
    if((ucRcvAddress == ucMBAddress) || (ucRcvAddress==MB_ADDRESS_BROADCAST)){
	    ucFunctionCode = *(ucMBFrame+MB_PDU_FUNC_OFF);
      eException = MB_EX_ILLEGAL_FUNCTION; 
      for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ ){
        //No more function handlers registered. Abort.
        if( xFuncHandlers[i].ucFunctionCode == 0 ){
          break;
        }
        else if( xFuncHandlers[i].ucFunctionCode == ucFunctionCode ){
          eException = xFuncHandlers[i].pxHandler( ucMBFrame, &usLength );
          break;
        }
      }
      if( ucRcvAddress != MB_ADDRESS_BROADCAST ) {        
        if( eException != MB_EX_NONE ) {        
          //An exception occured. Build an error frame.        
          usLength = 0;        
          ucMBFrame[usLength++] = ( UCHAR )( ucFunctionCode | MB_FUNC_ERROR );        
          ucMBFrame[usLength++] = eException;        
        }    
        sMBSlave.send( ucMBAddress, ucMBFrame, usLength );        
      } 
		}
	}
	memset(USART_DMA_RxBuf,0,USART1_DMA_Rec_Cnt);
  USART1_DMA_Rec_Cnt =0;
}

void MB_Slave_Send(UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength ){ 
  USHORT          usCRC16; 
	
  pucSndBufferCur = ( UCHAR * ) pucFrame - 1;
  usSndBufferCount = 1;      
  
  /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */      
  pucSndBufferCur[0] = ucSlaveAddress;      
  usSndBufferCount += usLength;      
  
  /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */      
  usCRC16 = usMBCRC16( ( UCHAR * ) pucSndBufferCur, usSndBufferCount );      
  ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 & 0xFF );      
  ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 >> 8 ); 
	
	memcpy(USART_DMA_TxBuf,(const void *)pucSndBufferCur,usSndBufferCount);
	USART1_DMA_Send_State = 1; 
  DMA_SetCurrDataCounter(DMA1_Channel4,usSndBufferCount); 
  DMA_Cmd(DMA1_Channel4,ENABLE); 
	while(USART1_DMA_Send_State); 
	usSndBufferCount = 0;
	/*
  if( usSndBufferCount != 0 ) {
    xMBPortSerialPutByte( ( CHAR )*pucSndBufferCur );
    pucSndBufferCur++;  
    usSndBufferCount--;
  }
  else{
    xNeedPoll = xMBPortEventPost( EV_FRAME_SENT );
    //Disable transmitter. This prevents another transmit buffer empty interrupt. 
    vMBPortSerialEnable( TRUE, FALSE );
  }	*/
}

void MB_Slave_DMA_TX_IRQHandler(void){
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
  DMA_ClearFlag(DMA1_FLAG_TC4);
  DMA_Cmd(DMA1_Channel4,DISABLE);
	USART1_DMA_Send_State = 0;
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
}
 
void MB_Slave_USART_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
  if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET){
		xSemaphoreGiveFromISR(BinarySemaphore_MB,&xHigherPriorityTaskWoken);
		USART1_DMA_Rec_Cnt = MB_UART_DEV->SR;
    USART1_DMA_Rec_Cnt = MB_UART_DEV->DR; //clear USART_IT_IDLE flag
		DMA_Cmd(DMA1_Channel5,DISABLE);
		USART1_DMA_Rec_Cnt = 256-DMA_GetCurrDataCounter(DMA1_Channel5);
    DMA_SetCurrDataCounter(DMA1_Channel5,256);
    DMA_Cmd(DMA1_Channel5,ENABLE);
	  xSemaphoreGiveFromISR(BinarySemaphore_MB,&xHigherPriorityTaskWoken);
	}
	USART_ClearITPendingBit(USART1,USART_IT_IDLE);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
