#ifndef __HW_SE_PORT_H
#define __HW_SE_PORT_H
 
float Bat_Read(void);
float Vref_Read(void);
float SE_AdcValue_Read(unsigned char chan); 
void SE_ADC_Init(void);
void SE_Chan_Open(unsigned char chan);
void SE_Chan_Close(void);
 
void SE_OUT_Init(unsigned char chan); 
void SE_OUT_Ctrl(unsigned char chan,unsigned char state);
#endif


