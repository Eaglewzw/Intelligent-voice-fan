/*------------------------------------
����:ds18b20����������ʾģ��
��д:YuMin_Dong
ʱ��:2013.01.21
------------------------------------*/

#ifndef _DS18B20_H_
#define _DS18B20_H_

/*------------------------------------
			λ����
------------------------------------*/
sbit DQ=P2^3;


UINT16 temperature;				//��ȡ�¶ȱ����ڴ�,�����ݴ���ʾʱ���λ��ʾ

/*------------------------------------
			��ʼ��18B20
------------------------------------*/
bit DS18b20_Init()
{
bit ok;
DQ=1;
DelayUs2x(5);
DQ=0;
DelayUs2x(200);
DelayUs2x(200); 
DQ=1;//�ͷ����ߣ�׼������������Ӧ
DelayUs2x(50);
ok=DQ;
DelayUs2x(25);
return ok;
}

/*------------------------------------
			��18b20д����
------------------------------------*/
void DS18b20_WriteByte(UINT8 byte)
{
UINT8 i;
	for(i=0;i<8;i++)
	{
	DQ=0;
	DelayUs2x(5);//���ֶ�д���ö���ʱ���ȥ��
	DQ=byte&0x01;	
	DelayUs2x(15);//����Ч�����ȶ�
	DQ=1;//��ƽ�ȶ����ͷ�����
	DelayUs2x(5);
	byte>>=1;
	}
DelayUs2x(25);//��оƬʱ�������
}

/*------------------------------------
			��18b20������
------------------------------------*/
UINT8 DS18b20_ReadByte()
{
UINT8 byte,i;
	for(i=0;i<8;i++)
	{
	DQ=0;
	byte>>=1;
	DQ=1;
		if(DQ)//ִ�е��˴���ǰ�������Ѿ�����ƽ�ȶ��ˣ����������ʱ
		byte|=0x80;
	DelayUs2x(20);	
	}
return byte;
}

/*------------------------------------
			���¶�
------------------------------------*/
UINT16 DS18b20_ReadTemperature()
{
UINT16 b;
UINT8 a;
DS18b20_Init();
DS18b20_WriteByte(0xcc);//skip rom ����ROM���кż��
DS18b20_WriteByte(0x44);//convert temperature ת�������¶�
DelayMs(10);//��оƬʱ�������ת���¶ȵ�ʱ��
DS18b20_Init();
DS18b20_WriteByte(0xcc); 
DS18b20_WriteByte(0xbe);//��ȡ�ݴ���
a=DS18b20_ReadByte();//�Ͱ�λ
b=DS18b20_ReadByte();//�߰�λ
b<<=8;
b=(b+a)*0.625;		//�Ŵ�ʮ������b��
return b;
}

/*------------------------------------
			�¶���ʾ
------------------------------------*/
void Display_Temperature()
{
temperature=DS18b20_ReadTemperature();	//��ȡ�¶ȵ�ת��������
/*
LCD_SendImg16x16(0,3,User4_16x16,0);	//��ʾ�¶�ͼ��
LCD_SendImg16x16(3,3,User2_16x16,0);*/	//��ʾ��
LCD_WriteCommand(0x30);
LCD_WriteCommand(0x98);
LCD_DisplayCGRAM(3);
LCD_WriteCommand(0x9B);
LCD_DisplayCGRAM(2);
LCD_WriteCommand(0x06);
LCD_WriteCommand(0x99);
LCD_WriteData(temperature%1000/100|0x30);
LCD_WriteData(temperature%100/10|0x30);
LCD_WriteData('.');
LCD_WriteData(temperature%10|0x30);
}

#endif