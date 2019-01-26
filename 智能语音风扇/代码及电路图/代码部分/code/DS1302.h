#ifndef _DS1302_H
#define _DS1302_H

#include "config.h"
#define uchar unsigned char
#define uint  unsigned int
sbit sclk=P2^4;
sbit io=P2^3;
sbit rst=P3^3;

//extern uchar time[];//��������ʱ����	

//extern uchar write_add[];
//extern uchar read_add[];
void write_ds1302_byte(uchar date);
void write_ds1302(uchar add,uchar date);
uchar read_ds1302(uchar add);
void set_time();			//��ʱ��д��ȥ
void read_time();	        //��ʱ�估BCMתʮ������

uchar Conver_week(uchar cyear,uchar cmonth,uchar cday);


#endif
