#include "delay.h"

void Delay200us(void)		//@22.1184MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 5;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

void  delay(unsigned long uldata)//@22.1184MHz
{
	while(uldata--)
	Delay200us();
}

void Delay200ms(void)		//@22.1184MHz
{
	unsigned char i, j, k;

	i = 17;
	j = 208;
	k = 27;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}





/*------------------------------------------------
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS 
------------------------------------------------*/
//void DelayUs2x(unsigned char t)
//{ 
//  while(--t);
//}
/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
//void DelayMs(unsigned char t)
//{
//    while(t--)
// 	{
//    	 //大致延时1mS
//    	 DelayUs2x(245);
//		 DelayUs2x(245);
// 	}
//}


void LCD_Delay(uint ms)
{
 while(ms--)
	{
      uchar i;
	  for(i=0;i<250;i++)  
	   {
	    _nop_();_nop_();
		_nop_();_nop_();
	   }
	}
}
