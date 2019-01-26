#ifndef _LCD12864_H_
#define _LCD12864_H_

#include "config.h"
//#include "intrins.h"
//#include "DS1302.h"
//#include "DS18B20.h"
#include <stdio.h>


#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};


sbit LCD_RS=P2^7;            //�Ĵ���ѡ������ 
sbit LCD_RW=P2^6;            //Һ����/д����
sbit LCD_EN=P2^5;            //Һ��ʹ�ܿ���
//===================����1==========================
/************************************************************************************/
extern unsigned char code num16321[11][64];
extern uchar code kong3216[];
extern uchar code table_week[12];
extern uchar code Dot[8];
extern uchar code NumSmall[10][18];

void displaypicture_12864(uchar y,uchar x,uchar *picture,uchar hight,uchar wide);
void ClearScreen(void);
void busy(void);
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar dat);
void lcd_init(void);
//void Draw_PM(const uchar *ptr);
void display_12864(uchar y,uchar x,uchar *z);//��ʾ�ַ�
void displayDigital_12864(uchar y,uchar x,uchar shu);//��ʾ����
void DisplayTime();


#endif