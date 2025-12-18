
#ifndef __CONFIG_H
#define __CONFIG_H
/*****相关宏定义***/
    /* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       long int int64_t;

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       long int uint64_t;


#define ENABLE  1
#define DISABLE  0
#define uchar unsigned char
#define uint  unsigned int
#define MAIN_Fosc  22118400UL


/****头文件包含*****/
#include "intrins.h"
#include "STC11XX.H"
#include "LDChip.h"
#include "Reg_RW.h"
#include "usart.h"
#include "DS18B20.h"
#include "LCD12864.h"
#include "DS1302.h"
#include "delay.h"
#include "SYN6288.h"



sbit LED=P4^2;//信号指示灯



/****条件编译定义*******/
#define TEST		 //测试命令



#endif
