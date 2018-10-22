#include "iic.h"


/******************************************************************************
* Function Name --> IIC����
* Description   --> SCL�ߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽͻ�䵽�͵�ƽʱ��������
*                   SCL: __________
*                                  \__________
*                   SDA: _____
*                             \_______________
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Start(void)
{
	Set_IIC_SDA_Output();	//���ó����

	IIC_SDA_1;	//ΪSDA�½�������׼��
	IIC_SCL_1;	//��SCL�ߵ�ƽʱ��SDAΪ�½���ʱ����������
	
#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */
	
	IIC_Delay();
	IIC_SDA_0;	//ͻ�䣬��������
	IIC_Delay();
	IIC_SCL_0;
	IIC_Delay();

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */
	
	IIC_SDA_0;	//ͻ�䣬��������
	IIC_SCL_0;

#endif  /* end __USER_DELAY_CLK */
}
/******************************************************************************
* Function Name --> IICֹͣ
* Description   --> SCL�ߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽͻ�䵽�ߵ�ƽʱֹͣ����
*                   SCL: ____________________
*                                  __________
*                   SDA: _________/
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Stop(void)
{
	Set_IIC_SDA_Output();	//���ó����

	IIC_SDA_0;	//ΪSDA������׼��
	
#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */

	IIC_Delay();
	IIC_SCL_1;	//��SCL�ߵ�ƽʱ��SDAΪ������ʱ������ֹͣ
	IIC_Delay();
	IIC_SDA_1;	//ͻ�䣬����ֹͣ
	IIC_Delay();

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */

	IIC_SCL_1;	//��SCL�ߵ�ƽʱ��SDAΪ������ʱ������ֹͣ
	IIC_SDA_1;	//ͻ�䣬����ֹͣ

#endif  /* end __USER_DELAY_CLK */
}
/******************************************************************************
* Function Name --> ������ӻ�����Ӧ���ź�
* Description   --> none
* Input         --> a��Ӧ���ź�
*                      0��Ӧ���ź�
*                      1����Ӧ���ź�
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Ack(unsigned char a)
{
	Set_IIC_SDA_Output();	//���ó����

	if(a)	IIC_SDA_1;	//����Ӧ���źŵ�ƽ
	else	IIC_SDA_0;
	
#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */

	IIC_Delay();
	IIC_SCL_1;	//ΪSCL�½���׼��
	IIC_Delay();
	IIC_SCL_0;	//ͻ�䣬��Ӧ���źŷ��͹�ȥ
	IIC_Delay();

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */

	IIC_SCL_1;	//ΪSCL�½���׼��
	IIC_SCL_0;	//ͻ�䣬��Ӧ���źŷ��͹�ȥ

#endif  /* end __USER_DELAY_CLK */
}
/******************************************************************************
* Function Name --> ��IIC���߷���һ���ֽ�����
* Description   --> none
* Input         --> dat��Ҫ���͵�����
* Output        --> none
* Reaturn       --> ack������Ӧ���ź�
******************************************************************************/
unsigned char IIC_Write_Byte(unsigned char dat)
{
	unsigned char i;
	unsigned char iic_ack=0;	//iicӦ���־

	Set_IIC_SDA_Output();	//���ó����

	for(i = 0;i < 8;i++)
	{
		if(dat & 0x80)	IIC_SDA_1;	//�жϷ���λ���ȷ��͸�λ
		else	IIC_SDA_0;

#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */
			
		IIC_Delay();
		IIC_SCL_1;	//ΪSCL�½���׼��
		IIC_Delay();
		IIC_SCL_0;	//ͻ�䣬������λ���͹�ȥ
		dat<<=1;	//��������һλ
	}	//�ֽڷ�����ɣ���ʼ����Ӧ���ź�

	IIC_SDA_1;	//�ͷ�������

	Set_IIC_SDA_Input();	//���ó�����

	IIC_Delay();
	IIC_SCL_1;	//ΪSCL�½���׼��
	IIC_Delay();

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */

		IIC_SCL_1;	//ΪSCL�½���׼��
		IIC_SCL_0;	//ͻ�䣬������λ���͹�ȥ
		dat<<=1;	//��������һλ
	}	//�ֽڷ�����ɣ���ʼ����Ӧ���ź�

	IIC_SDA_1;	//�ͷ�������

	Set_IIC_SDA_Input();	//���ó�����

	IIC_SCL_1;	//ΪSCL�½���׼��

#endif  /* end __USER_DELAY_CLK */
	
	iic_ack |= IIC_SDA_IN;	//����Ӧ��λ
	IIC_SCL_0;
	return iic_ack;	//����Ӧ���ź�
}
/******************************************************************************
* Function Name --> ��IIC�����϶�ȡһ���ֽ�����
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> x����ȡ��������
******************************************************************************/
unsigned char IIC_Read_Byte(void)
{
	unsigned char i;
	unsigned char x=0;

	IIC_SDA_1;	//������������Ϊ�ߵ�ƽ

	Set_IIC_SDA_Input();	//���ó�����

	for(i = 0;i < 8;i++)
	{
		x <<= 1;	//�������ݣ���λ��ǰ

#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */

		IIC_Delay();
		IIC_SCL_1;	//ͻ��
		IIC_Delay();
		
		if(IIC_SDA_IN)	x |= 0x01;	//�յ��ߵ�ƽ

		IIC_SCL_0;
		IIC_Delay();
	}	//���ݽ������

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */

		IIC_SCL_1;	//ͻ��
		
		if(IIC_SDA_IN)	x |= 0x01;	//�յ��ߵ�ƽ

		IIC_SCL_0;
	}	//���ݽ������

#endif  /* end __USER_DELAY_CLK */

	IIC_SCL_0;

	return x;	//���ض�ȡ��������
}

/******************************************************************************
* Function Name --> GPIO��ʼ��
* Description   --> STM32ƽ̨������ƽ̨���޸Ļ���ע�͵�
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_PCF_GPIO_Init(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	IIC_SDA_1;
	IIC_SCL_1;
}

void Set_IIC_SDA_Output(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Set_IIC_SDA_Input(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


