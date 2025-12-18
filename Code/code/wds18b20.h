/*------------------------------------
名称:ds18b20驱动及其显示模块
编写:YuMin_Dong
时间:2013.01.21
------------------------------------*/

#ifndef _DS18B20_H_
#define _DS18B20_H_

/*------------------------------------
			位定义
------------------------------------*/
sbit DQ=P2^3;


UINT16 temperature;				//读取温度保存在此,用作暂存显示时候分位显示

/*------------------------------------
			初始化18B20
------------------------------------*/
bit DS18b20_Init()
{
bit ok;
DQ=1;
DelayUs2x(5);
DQ=0;
DelayUs2x(200);
DelayUs2x(200); 
DQ=1;//释放总线，准备接收器件回应
DelayUs2x(50);
ok=DQ;
DelayUs2x(25);
return ok;
}

/*------------------------------------
			向18b20写数据
------------------------------------*/
void DS18b20_WriteByte(UINT8 byte)
{
UINT8 i;
	for(i=0;i<8;i++)
	{
	DQ=0;
	DelayUs2x(5);//区分读写，让读的时间过去。
	DQ=byte&0x01;	
	DelayUs2x(15);//让有效数据稳定
	DQ=1;//电平稳定后，释放总线
	DelayUs2x(5);
	byte>>=1;
	}
DelayUs2x(25);//给芯片时间存数据
}

/*------------------------------------
			从18b20读数据
------------------------------------*/
UINT8 DS18b20_ReadByte()
{
UINT8 byte,i;
	for(i=0;i<8;i++)
	{
	DQ=0;
	byte>>=1;
	DQ=1;
		if(DQ)//执行到此处，前面的语句已经够电平稳定了，不用另加延时
		byte|=0x80;
	DelayUs2x(20);	
	}
return byte;
}

/*------------------------------------
			读温度
------------------------------------*/
UINT16 DS18b20_ReadTemperature()
{
UINT16 b;
UINT8 a;
DS18b20_Init();
DS18b20_WriteByte(0xcc);//skip rom 跳过ROM序列号检查
DS18b20_WriteByte(0x44);//convert temperature 转换数字温度
DelayMs(10);//给芯片时间采样，转换温度的时间
DS18b20_Init();
DS18b20_WriteByte(0xcc); 
DS18b20_WriteByte(0xbe);//读取暂存器
a=DS18b20_ReadByte();//低八位
b=DS18b20_ReadByte();//高八位
b<<=8;
b=(b+a)*0.625;		//放大十倍放在b中
return b;
}

/*------------------------------------
			温度显示
------------------------------------*/
void Display_Temperature()
{
temperature=DS18b20_ReadTemperature();	//读取温度到转换变量里
/*
LCD_SendImg16x16(0,3,User4_16x16,0);	//显示温度图标
LCD_SendImg16x16(3,3,User2_16x16,0);*/	//显示℃
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