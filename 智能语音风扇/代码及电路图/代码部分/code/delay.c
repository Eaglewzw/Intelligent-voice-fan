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
 uS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��,������ʱ
 �������� T=tx2+5 uS 
------------------------------------------------*/
//void DelayUs2x(unsigned char t)
//{ 
//  while(--t);
//}
/*------------------------------------------------
 mS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
------------------------------------------------*/
//void DelayMs(unsigned char t)
//{
//    while(t--)
// 	{
//    	 //������ʱ1mS
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
