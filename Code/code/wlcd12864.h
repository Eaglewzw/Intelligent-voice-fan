/*------------------------------------
名称:LCD9012驱动模块
编写:YuMin_Dong
时间:2013.01.21
------------------------------------*/

#ifndef _LCD12864_H_
#define _LCD12864_H_

/*------------------------------------
			I/O位定义
------------------------------------*/
sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P2^2;

/*------------------------------------
			宏定义
------------------------------------*/
#define DataPort P1
#define RS_CLR RS=0
#define RS_SET RS=1
#define RW_CLR RW=0
#define RW_SET RW=1
#define EN_CLR EN=0
#define EN_SET EN=1
//#define PSB_CLR PSB=0
//#define PSB_SET PSB=1
//#define RST_CLR RST=0
//#define RST_SET RST=1

#define MENU_NUM 7		//表示有主菜单有7个选项

UINT8 guide,guide_old=1;	//主菜单导航箭头定义,guide_old在有按键前等于guide,有修改后guide_old变化用来作为刷新显示的条件

/*------------------------------------
			主菜单内容
------------------------------------*/
code UINT8 *MainMenu[]={"  1.时间设置","  2.日期设置","  3.闹钟功能","  4.背光开关","  5.键音开关","  6.生日提醒","  7.返回"};

/*------------------------------------
			判忙函数
------------------------------------*/
void LCD_CheckBusy()
{
RS_CLR;
RW_SET;
EN_SET;
DataPort=0xff;
while(DataPort&0x80)
;
EN_CLR;
}

/*------------------------------------
			写命令函数
------------------------------------*/
void LCD_WriteCommand(UINT8 byte)
{
LCD_CheckBusy();
RS_CLR;
RW_CLR;
EN_SET;
DataPort=byte;
DelayUs2x(5);
EN_CLR;
DelayUs2x(5);
}

/*------------------------------------
			写数据函数 
------------------------------------*/
void LCD_WriteData(UINT8 byte)
{
LCD_CheckBusy();
RS_SET;
RW_CLR;
EN_SET;
DataPort=byte;
DelayUs2x(5);
EN_CLR;
DelayUs2x(5);
}

/*------------------------------------
			LCD初始化
------------------------------------*/
void LCD_Init()
{
//DelayMs(30);
//PSB_SET;	//设置并口模式
//DelayMs(1);
//RST_CLR;
//DelayMs(1);
//RST_SET;
//DelayMs(10);
LCD_WriteCommand(0x30);	//选择基础指令集
DelayUs2x(50);			//延时大于100us
//LCD_WriteCommand(0x30);	//
//DelayUs2x(20);			//延时大于37us
LCD_WriteCommand(0x0c);	//开始显示,无反白,无游标
DelayUs2x(50);			//延时大于100us
LCD_WriteCommand(0x01);	//清屏
DelayMs(5);			
//LCD_WriteCommand(0x06);	//读取或写入时,游标移动方向,设定从右向左
//DelayUs2x(50);			//延时大于100us
}

/*------------------------------------
			设置坐标
------------------------------------*/
/*void LCD_SetXY(UINT8 x,UINT8 y)
{
	switch(y)
	{
	case 0: LCD_WriteCommand(0x80+x);
			break;
	case 1: LCD_WriteCommand(0x90+x);
			break;
	case 2: LCD_WriteCommand(0x88+x);
			break;
	case 3: LCD_WriteCommand(0x98+x);
			break;
	default:break;
	}
}*/

/*------------------------------------
			写字符串
------------------------------------*/
void LCD_SendString(UINT8 *s)
{
//LCD_SetXY(x,y);
	while(*s&&s)
	{
	LCD_WriteData(*s);
	s++;
	}
}

/*------------------------------------
			设置CGRAM
------------------------------------*/
void LCD_SetCGRAM(UINT8 n,UINT8 *pic)	//第n个自定义字符
{
UINT8 i;
LCD_WriteCommand(0x30);
LCD_WriteCommand(0x40+n*16);
	for(i=0;i<16;i++)
	{
	LCD_WriteData(*(pic+2*i));
	LCD_WriteData(*(pic+2*i+1));
	}
}

/*------------------------------------
			显示CGRAM
------------------------------------*/
void LCD_DisplayCGRAM(UINT8 n)
{
//LCD_WriteCommand(0x30);
LCD_WriteData(0x00);
LCD_WriteData(0x02*n);
}

/*------------------------------------
			GDRAM绘16x16图
------------------------------------
void LCD_SendImg16x16(UINT8 x,UINT8 y,UINT8 *img,bit g)		//(x,y)坐标 img指向16x16字模数组,g为是否反白
{
UINT8 i,j,base_x;
LCD_WriteCommand(0x34);
LCD_WriteCommand(0x36);
	if(y==0||y==1)
	{
	base_x=0x80;
	y=y*16;
	}
	else 
	{
	base_x=0x88;
	y=(y-2)*16;
	}
	for(i=0;i<16;i++)	
	{															
		LCD_WriteCommand(0x80+y+i); 
		LCD_WriteCommand(base_x+x);  
		for(j=0;j<2;j++)  
		{
			if(g)
				LCD_WriteData(~(*img++));
			else
				LCD_WriteData(*img++);
		}
	}	
LCD_WriteCommand(0x36);
//LCD_WriteCommand(0x30);
}*/


/*------------------------------------
			GDRAM绘16x32图
------------------------------------*/
void LCD_SendImg16x32(UINT8 x,UINT8 y,UINT8 *img,bit g)//x标志表示是否反白。
{
UINT8 i,j,base_x;
LCD_WriteCommand(0x34);
LCD_WriteCommand(0x36);
switch(y)
{
case 0: base_x=0x80;break;
case 1: base_x=0x90;break;
case 2: base_x=0x88;break;
default:return;
}
	if(y==0||y==2)
	{
		for(i=0;i<32;i++)
		{
		LCD_WriteCommand(0x80+i);
		LCD_WriteCommand(base_x+x);
			for(j=0;j<2;j++)
			{
			if(!g)
			LCD_WriteData(*img++);
			else LCD_WriteData(~(*img++));
			}
		}
	}
	if(y==1)
	{
		for(i=0;i<16;i++)
		{
		LCD_WriteCommand(0x90+i);
		LCD_WriteCommand(base_x+x);
			for(j=0;j<2;j++)
			{
			if(g)
			LCD_WriteData(~(*img++));
			else
			LCD_WriteData(*img++);
			}
		}
		for(i=0;i<16;i++)
		{
		LCD_WriteCommand(0x80+i);
		LCD_WriteCommand(base_x+x+8);
			for(j=0;j<2;j++)
			{
			if(g)
			LCD_WriteData(~(*img++));
			else
			LCD_WriteData(*img++);
			}
		}
	}
LCD_WriteCommand(0x36);
//LCD_WriteCommand(0x30);
}

/*------------------------------------
			GDRAM绘制128x64
------------------------------------*/
void LCD_SendImg128x64(UINT8 *img)
{
UINT8 i,j;
//LCD_WriteCommand(0x30);
LCD_WriteCommand(0x36);
	for(i=0;i<32;i++)
	{
	LCD_WriteCommand(0x80+i);
	LCD_WriteCommand(0x80);
		for(j=0;j<16;j++)
		{
		LCD_WriteData(*img++);
		}
	}
	for(i=0;i<32;i++)
	{
	LCD_WriteCommand(0x80+i);
	LCD_WriteCommand(0x88);
		for(j=0;j<16;j++)
		{
		LCD_WriteData(*img++);
		}
	}
LCD_WriteCommand(0x36);
}

/*------------------------------------
			清除GDRAM
------------------------------------*/
void LCD_ClearGDRAM()
{
UINT8 i,j;
LCD_WriteCommand(0x30);			//一般指令
LCD_WriteCommand(0x0C); 		//开启显示
LCD_WriteCommand(0x06); 		//AC++
LCD_WriteCommand(0x01);			//清DDRAM
LCD_WriteCommand(0x36);			//开启绘图
//LCD_InitGDRAM();
LCD_WriteCommand(0x36);			//显示绘图
	for(i=0;i<32;i++)			//在DGRAM里写满0,表示清除GDRAM
	{
	LCD_WriteCommand(0x80+i);
	LCD_WriteCommand(0x80);
		for(j=0;j<16;j++)
		{
		LCD_WriteData(0x00);
		LCD_WriteData(0x00);
		}
	}
LCD_WriteCommand(0x36);		//绘图完毕,开启显示
}

/*------------------------------------
			清除1,2,3行屏
------------------------------------*/
void LCD_ClearScreen123()
{
LCD_WriteCommand(0x30);
	LCD_WriteCommand(0x90);
	LCD_SendString("                ");
	LCD_WriteCommand(0x88);
	LCD_SendString("                ");
	LCD_WriteCommand(0x98);
	LCD_SendString("                ");
}

/*------------------------------------
			菜单选择更新
------------------------------------*/
void LCD_RefreshMenu()
{
LCD_ClearScreen123();
	LCD_WriteCommand(0x90);
	if(0+(guide/3)*3<MENU_NUM)
	LCD_SendString(*(MainMenu+(guide/3)*3));
	
	LCD_WriteCommand(0x88);
	if(1+(guide/3)*3<MENU_NUM)
	LCD_SendString(*(MainMenu+(guide/3)*3+1));
	
	LCD_WriteCommand(0x98);
	if(2+(guide/3)*3<MENU_NUM)
	LCD_SendString(*(MainMenu+(guide/3)*3+2));
	
	switch(guide%3)
	{
	case 0: LCD_WriteCommand(0x90);break;
	case 1: LCD_WriteCommand(0x88);break;
	case 2: LCD_WriteCommand(0x98);break;
	default:return ;
	}
	LCD_DisplayCGRAM(1);
}

#endif