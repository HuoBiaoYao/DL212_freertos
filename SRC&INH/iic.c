#include "iic.h"


/******************************************************************************
* Function Name --> IIC启动
* Description   --> SCL高电平期间，SDA由高电平突变到低电平时启动总线
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
	Set_IIC_SDA_Output();	//设置成输出

	IIC_SDA_1;	//为SDA下降启动做准备
	IIC_SCL_1;	//在SCL高电平时，SDA为下降沿时候总线启动
	
#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */
	
	IIC_Delay();
	IIC_SDA_0;	//突变，总线启动
	IIC_Delay();
	IIC_SCL_0;
	IIC_Delay();

#else  /* 否则不使用延时函数改变通讯频率 */
	
	IIC_SDA_0;	//突变，总线启动
	IIC_SCL_0;

#endif  /* end __USER_DELAY_CLK */
}
/******************************************************************************
* Function Name --> IIC停止
* Description   --> SCL高电平期间，SDA由低电平突变到高电平时停止总线
*                   SCL: ____________________
*                                  __________
*                   SDA: _________/
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Stop(void)
{
	Set_IIC_SDA_Output();	//设置成输出

	IIC_SDA_0;	//为SDA上升做准备
	
#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */

	IIC_Delay();
	IIC_SCL_1;	//在SCL高电平时，SDA为上升沿时候总线停止
	IIC_Delay();
	IIC_SDA_1;	//突变，总线停止
	IIC_Delay();

#else  /* 否则不使用延时函数改变通讯频率 */

	IIC_SCL_1;	//在SCL高电平时，SDA为上升沿时候总线停止
	IIC_SDA_1;	//突变，总线停止

#endif  /* end __USER_DELAY_CLK */
}
/******************************************************************************
* Function Name --> 主机向从机发送应答信号
* Description   --> none
* Input         --> a：应答信号
*                      0：应答信号
*                      1：非应答信号
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Ack(unsigned char a)
{
	Set_IIC_SDA_Output();	//设置成输出

	if(a)	IIC_SDA_1;	//放上应答信号电平
	else	IIC_SDA_0;
	
#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */

	IIC_Delay();
	IIC_SCL_1;	//为SCL下降做准备
	IIC_Delay();
	IIC_SCL_0;	//突变，将应答信号发送过去
	IIC_Delay();

#else  /* 否则不使用延时函数改变通讯频率 */

	IIC_SCL_1;	//为SCL下降做准备
	IIC_SCL_0;	//突变，将应答信号发送过去

#endif  /* end __USER_DELAY_CLK */
}
/******************************************************************************
* Function Name --> 向IIC总线发送一个字节数据
* Description   --> none
* Input         --> dat：要发送的数据
* Output        --> none
* Reaturn       --> ack：返回应答信号
******************************************************************************/
unsigned char IIC_Write_Byte(unsigned char dat)
{
	unsigned char i;
	unsigned char iic_ack=0;	//iic应答标志

	Set_IIC_SDA_Output();	//设置成输出

	for(i = 0;i < 8;i++)
	{
		if(dat & 0x80)	IIC_SDA_1;	//判断发送位，先发送高位
		else	IIC_SDA_0;

#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */
			
		IIC_Delay();
		IIC_SCL_1;	//为SCL下降做准备
		IIC_Delay();
		IIC_SCL_0;	//突变，将数据位发送过去
		dat<<=1;	//数据左移一位
	}	//字节发送完成，开始接收应答信号

	IIC_SDA_1;	//释放数据线

	Set_IIC_SDA_Input();	//设置成输入

	IIC_Delay();
	IIC_SCL_1;	//为SCL下降做准备
	IIC_Delay();

#else  /* 否则不使用延时函数改变通讯频率 */

		IIC_SCL_1;	//为SCL下降做准备
		IIC_SCL_0;	//突变，将数据位发送过去
		dat<<=1;	//数据左移一位
	}	//字节发送完成，开始接收应答信号

	IIC_SDA_1;	//释放数据线

	Set_IIC_SDA_Input();	//设置成输入

	IIC_SCL_1;	//为SCL下降做准备

#endif  /* end __USER_DELAY_CLK */
	
	iic_ack |= IIC_SDA_IN;	//读入应答位
	IIC_SCL_0;
	return iic_ack;	//返回应答信号
}
/******************************************************************************
* Function Name --> 从IIC总线上读取一个字节数据
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> x：读取到的数据
******************************************************************************/
unsigned char IIC_Read_Byte(void)
{
	unsigned char i;
	unsigned char x=0;

	IIC_SDA_1;	//首先置数据线为高电平

	Set_IIC_SDA_Input();	//设置成输入

	for(i = 0;i < 8;i++)
	{
		x <<= 1;	//读入数据，高位在前

#if _USER_DELAY_CLK==1  /* 定义了则使用延时函数来改变通讯频率 */

		IIC_Delay();
		IIC_SCL_1;	//突变
		IIC_Delay();
		
		if(IIC_SDA_IN)	x |= 0x01;	//收到高电平

		IIC_SCL_0;
		IIC_Delay();
	}	//数据接收完成

#else  /* 否则不使用延时函数改变通讯频率 */

		IIC_SCL_1;	//突变
		
		if(IIC_SDA_IN)	x |= 0x01;	//收到高电平

		IIC_SCL_0;
	}	//数据接收完成

#endif  /* end __USER_DELAY_CLK */

	IIC_SCL_0;

	return x;	//返回读取到的数据
}

/******************************************************************************
* Function Name --> GPIO初始化
* Description   --> STM32平台，其他平台请修改或者注释掉
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


