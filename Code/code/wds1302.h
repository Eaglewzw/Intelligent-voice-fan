/*------------------------------------
名称:DS1302Driver
编写:YuMin_Dong
时间:2013.01.20
------------------------------------*/
#ifndef _DS1302_H_
#define _DS1302_H_

/*------------------------------------
			I/O位定义
------------------------------------*/
sbit DS_RST=P2^4;
sbit DS_SCL=P2^6;
sbit DS_IO=P2^5;

/*------------------------------------
			宏定义
------------------------------------*/
#define DS_RST_CLR DS_RST=0
#define DS_SCL_CLR DS_SCL=0
#define DS_IO_CLR  DS_IO=0
#define DS_IO_SET DS_IO=1
#define DS_SCL_SET DS_SCL=1
#define DS_RST_SET DS_RST=1

#define SECOND_ADDRESS 0x80		//秒 寄存地址
#define MINUTE_ADDRESS 0x82		//分 寄存地址
#define HOUR_ADDRESS 0x84		//时 寄存地址
#define DATE_ADDRESS 0x86		//日 寄存地址
#define MONTH_ADDRESS 0x88		//月 寄存地址
#define WEEK_ADDRESS 0x8a		//周 寄存地址
#define YEAR_ADDRESS 0x8c		//年 寄存地址
#define CONTROL_ADDRESS 0x8e	//控制寄存器地址

/*------------------------------------
			全局变量定义
------------------------------------*/
UINT8 Year,Month,Date,Hour,Minute,Second,Week;	//年月日时分秒周的暂存
bit Set_Hour;				//为1表示设置时钟,为0表示设置分钟,秒钟在设置时间的时候停摆清零
UINT8 Set_Date;				//设置年月日选择标识
bit Menu0;					//一号子菜单调时位

UINT8 Set_Bell;
UINT8 Bell_Hour,Bell_Minute;

code UINT8 WEEK_TAB[12]={0,3,3,6,1,4,6,2,5,0,3,5};	//星期补偿

/*------------------------------------
			Write函数
------------------------------------*/
void DS1302_WriteByte(UINT8 address,UINT8 byte)
{
UINT8 i;
address=address&0xfe;	//R/W控制位低电位写
DS_RST_SET;
	for(i=0;i<8;i++)		//发送地址,找寄存器
	{
		if(address&0x01)
		DS_IO_SET;
		else DS_IO_CLR;
		address>>=1;
		DS_SCL_SET;
		DS_SCL_CLR;
	}
	for(i=0;i<8;i++)		//发送数据到寻找到的寄存器
	{
		if(byte&0x01)
		DS_IO_SET;
		else DS_IO_CLR;
		byte>>=1;
		DS_SCL_SET;
		DS_SCL_CLR;
	}
DS_RST_CLR;
}

/*------------------------------------
			Read函数
------------------------------------*/
UINT8 DS1302_ReadByte(UINT8 address)
{
UINT8 i,byte;
address=address|0x01;
DS_RST_SET;
	for(i=0;i<8;i++)		//发送地址,找到寄存器
	{
		if(address&0x01)
		DS_IO_SET;
		else DS_IO_CLR;
		address>>=1;
		DS_SCL_SET;
		DS_SCL_CLR;
	}
	for(i=0;i<8;i++)		//从找到的寄存器中读取数据
	{
		byte>>=1;
		if(DS_IO)
		byte|=0x80;
		DS_SCL_SET;
		DS_SCL_CLR;
	}
DS_RST_CLR;
return byte;
}

/*------------------------------------
			读取时间
------------------------------------*/
void DS1302_ReadTime()
{
Hour=DS1302_ReadByte(HOUR_ADDRESS);
Minute=DS1302_ReadByte(MINUTE_ADDRESS);
Second=DS1302_ReadByte(SECOND_ADDRESS)&0x7f;
}

/*------------------------------------
			写入时间
------------------------------------*/
void DS1302_WriteTime()
{
DS1302_WriteByte(CONTROL_ADDRESS,0X00);		//关闭写保护
DS1302_WriteByte(HOUR_ADDRESS,Hour);
DS1302_WriteByte(MINUTE_ADDRESS,Minute);	
DS1302_WriteByte(SECOND_ADDRESS,Second&0x7f);//清零秒
DS1302_WriteByte(CONTROL_ADDRESS,0x80);		//打开写保护
}

/*------------------------------------
			显示时间
------------------------------------*/
void Display_Time()
{
LCD_WriteCommand(0x34);				//16x32的时钟显示,先开始扩充指令模式
LCD_WriteCommand(0x36);				//开启绘图模式

LCD_SendImg16x32(0,1,*(User_16x32+Hour/16),0|Set_Hour);	
LCD_SendImg16x32(1,1,*(User_16x32+Hour%16),0|Set_Hour);

LCD_SendImg16x32(2,1,*(User_16x32+10),0);


LCD_SendImg16x32(3,1,*(User_16x32+Minute/16),0|(!Set_Hour&&Menu0));
LCD_SendImg16x32(4,1,*(User_16x32+Minute%16),0|(!Set_Hour&&Menu0));

LCD_SendImg16x32(5,1,*(User_16x32+10),0);


LCD_SendImg16x32(6,1,*(User_16x32+Second/16),0);
LCD_SendImg16x32(7,1,*(User_16x32+Second%16),0);
//LCD_WriteCommand(0x30);
}

/*------------------------------------
			设置闹铃界面填充
------------------------------------*/
void Display_Bell()
{
LCD_WriteCommand(0x34);				//16x32的时钟显示,先开始扩充指令模式
LCD_WriteCommand(0x36);				//开启绘图模式

LCD_SendImg16x32(0,1,*(User_16x32+Bell_Hour/10),Set_Bell==0);	
LCD_SendImg16x32(1,1,*(User_16x32+Bell_Hour%10),Set_Bell==0);

LCD_SendImg16x32(2,1,*(User_16x32+10),0);


LCD_SendImg16x32(3,1,*(User_16x32+Bell_Minute/10),Set_Bell==1);
LCD_SendImg16x32(4,1,*(User_16x32+Bell_Minute%10),Set_Bell==1);

LCD_SendImg16x32(5,1,*(User_16x32+10),0);

LCD_SendImg16x32(6,1,*User_16x32,0);
LCD_SendImg16x32(7,1,*User_16x32,0);
}

/*------------------------------------
			读取日期
------------------------------------*/
void DS1302_ReadDate()
{
Year=DS1302_ReadByte(YEAR_ADDRESS);	
Month=DS1302_ReadByte(MONTH_ADDRESS);
Date=DS1302_ReadByte(DATE_ADDRESS);		
}

/*------------------------------------
			写入日期
------------------------------------*/
void DS1302_WriteDate()
{
DS1302_WriteByte(CONTROL_ADDRESS,0X00);		//关闭写保护
DS1302_WriteByte(DATE_ADDRESS,Date);
DS1302_WriteByte(MONTH_ADDRESS,Month);	
DS1302_WriteByte(YEAR_ADDRESS,Year);//清零秒
DS1302_WriteByte(CONTROL_ADDRESS,0x80);		//打开写保护
}

/*------------------------------------
		设置时填充大数字日期
------------------------------------*/
void Display_SetDate()
{
LCD_WriteCommand(0x34);
LCD_WriteCommand(0x36);

LCD_SendImg16x32(0,1,*(User_16x32+Year/16),Set_Date==0);	
LCD_SendImg16x32(1,1,*(User_16x32+Year%16),Set_Date==0);

LCD_SendImg16x32(2,1,*(User_16x32+11),0);

LCD_SendImg16x32(3,1,*(User_16x32+Month/16),Set_Date==1);
LCD_SendImg16x32(4,1,*(User_16x32+Month%16),Set_Date==1);

LCD_SendImg16x32(5,1,*(User_16x32+11),0);

LCD_SendImg16x32(6,1,*(User_16x32+Date/16),Set_Date==2);
LCD_SendImg16x32(7,1,*(User_16x32+Date%16),Set_Date==2);
//LCD_WriteCommand(0x30);
}
/*------------------------------------
			星期转换
------------------------------------*/
UINT8 Conver_Week(bit c,UINT8 year,UINT8 month,UINT8 date) 
{
	UINT8 temp1,temp2,week; 
	temp1=year/16; 
	temp2=year%16; 
	year=temp1*10+temp2; 
	temp1=month/16; 
	temp2=month%16; 
	month=temp1*10+temp2; 
	temp1=date/16; 
	temp2=date%16; 
	date=temp1*10+temp2; 
	
	if (c==0){year+=0x64;} 
	temp1=year/0x4; 
	temp2=year+temp1; 
	temp2=temp2%0x7; 
	temp2=temp2+date+WEEK_TAB[month-1]; 
	if (year%0x4==0&&month<3)temp2-=1; 
	week=temp2%0x7; 
	return week;
} 
/*------------------------------------
			显示日期
------------------------------------*/
void Display_Date()
{
//if(Set_Year&&!Set_Month)
//Year=~Year;
LCD_WriteCommand(0x30);
LCD_WriteCommand(0x06);
						//显示日期格式20XX-XX-XX
LCD_WriteCommand(0x80); 
		//从1302读出年信息
LCD_SendString("20");
LCD_WriteData(Year/16|0x30);				//填充年信息
LCD_WriteData(Year%16|0x30);

LCD_WriteData('-');							//分隔符

	//从1302读出月信息
LCD_WriteData(Month/16|0x30);				//LCD填充月信息
LCD_WriteData(Month%16|0x30);

LCD_WriteData('-');							//分隔符

		//从1302读出日信息
LCD_WriteData(Date/16|0x30);				//LCD填充日信息
LCD_WriteData(Date%16|0x30);
}

/*------------------------------------
			换算并显示星期
------------------------------------*/
void Display_Week()
{
LCD_WriteCommand(0x86);
//Week=DS1302_ReadByte(WEEK_ADDRESS);
Week=Conver_Week(0,Year,Month,Date);
LCD_SendString("周");
	switch(Week)
	{
	case 0: LCD_SendString("日");
			break;
	case 1:	LCD_SendString("一");
			break;
	case 2: LCD_SendString("二"); 
			break;
	case 3:	LCD_WriteData(0xc8);		//字库区位码表得出"三"的显示码.
			LCD_WriteData(0xfd);
			break;
	case 4: LCD_SendString("四");
			break;
	case 5: LCD_SendString("五");
			break;
	case 6: LCD_SendString("六");
			break;
	default:return ;
	}
}

/*------------------------------------
			初始化
------------------------------------*/
void DS1302_Init()			//初始化DS1302
{
DS_SCL_CLR;
DS_RST_CLR;
//DS1302_WriteByte(CONTROL_ADDRESS,0X00);
//DS1302_WriteByte(HOUR_ADDRESS,0x16);
//DS1302_WriteByte(CONTROL_ADDRESS,0x80);

}
#endif