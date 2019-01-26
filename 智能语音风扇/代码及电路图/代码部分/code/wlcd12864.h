/*------------------------------------
����:LCD9012����ģ��
��д:YuMin_Dong
ʱ��:2013.01.21
------------------------------------*/

#ifndef _LCD12864_H_
#define _LCD12864_H_

/*------------------------------------
			I/Oλ����
------------------------------------*/
sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P2^2;

/*------------------------------------
			�궨��
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

#define MENU_NUM 7		//��ʾ�����˵���7��ѡ��

UINT8 guide,guide_old=1;	//���˵�������ͷ����,guide_old���а���ǰ����guide,���޸ĺ�guide_old�仯������Ϊˢ����ʾ������

/*------------------------------------
			���˵�����
------------------------------------*/
code UINT8 *MainMenu[]={"  1.ʱ������","  2.��������","  3.���ӹ���","  4.���⿪��","  5.��������","  6.��������","  7.����"};

/*------------------------------------
			��æ����
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
			д�����
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
			д���ݺ��� 
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
			LCD��ʼ��
------------------------------------*/
void LCD_Init()
{
//DelayMs(30);
//PSB_SET;	//���ò���ģʽ
//DelayMs(1);
//RST_CLR;
//DelayMs(1);
//RST_SET;
//DelayMs(10);
LCD_WriteCommand(0x30);	//ѡ�����ָ�
DelayUs2x(50);			//��ʱ����100us
//LCD_WriteCommand(0x30);	//
//DelayUs2x(20);			//��ʱ����37us
LCD_WriteCommand(0x0c);	//��ʼ��ʾ,�޷���,���α�
DelayUs2x(50);			//��ʱ����100us
LCD_WriteCommand(0x01);	//����
DelayMs(5);			
//LCD_WriteCommand(0x06);	//��ȡ��д��ʱ,�α��ƶ�����,�趨��������
//DelayUs2x(50);			//��ʱ����100us
}

/*------------------------------------
			��������
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
			д�ַ���
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
			����CGRAM
------------------------------------*/
void LCD_SetCGRAM(UINT8 n,UINT8 *pic)	//��n���Զ����ַ�
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
			��ʾCGRAM
------------------------------------*/
void LCD_DisplayCGRAM(UINT8 n)
{
//LCD_WriteCommand(0x30);
LCD_WriteData(0x00);
LCD_WriteData(0x02*n);
}

/*------------------------------------
			GDRAM��16x16ͼ
------------------------------------
void LCD_SendImg16x16(UINT8 x,UINT8 y,UINT8 *img,bit g)		//(x,y)���� imgָ��16x16��ģ����,gΪ�Ƿ񷴰�
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
			GDRAM��16x32ͼ
------------------------------------*/
void LCD_SendImg16x32(UINT8 x,UINT8 y,UINT8 *img,bit g)//x��־��ʾ�Ƿ񷴰ס�
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
			GDRAM����128x64
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
			���GDRAM
------------------------------------*/
void LCD_ClearGDRAM()
{
UINT8 i,j;
LCD_WriteCommand(0x30);			//һ��ָ��
LCD_WriteCommand(0x0C); 		//������ʾ
LCD_WriteCommand(0x06); 		//AC++
LCD_WriteCommand(0x01);			//��DDRAM
LCD_WriteCommand(0x36);			//������ͼ
//LCD_InitGDRAM();
LCD_WriteCommand(0x36);			//��ʾ��ͼ
	for(i=0;i<32;i++)			//��DGRAM��д��0,��ʾ���GDRAM
	{
	LCD_WriteCommand(0x80+i);
	LCD_WriteCommand(0x80);
		for(j=0;j<16;j++)
		{
		LCD_WriteData(0x00);
		LCD_WriteData(0x00);
		}
	}
LCD_WriteCommand(0x36);		//��ͼ���,������ʾ
}

/*------------------------------------
			���1,2,3����
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
			�˵�ѡ�����
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