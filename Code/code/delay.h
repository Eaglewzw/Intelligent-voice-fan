#ifndef _delay_H_
#define _delay_H_

#include "config.h"


void Delay200ms(void);
void delay(unsigned long uldata);
void Delay200us(void);		//@22.1184MHz

//void DelayUs2x(unsigned char t);
//void DelayMs(unsigned char t);
void LCD_Delay(uint ms);


#endif