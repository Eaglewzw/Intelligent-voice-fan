#include"DS18B20.h"


void delay_us(uchar us)
{
	uint i=2;
	while(i--)
	while(us--); 
}

bit ds_init()//�����߳�ʼ��ʱ��
{
	bit i;
	ds=1;
	_nop_();
	ds=0;
	delay_us(75);
	ds=1;
	delay_us(4);
	i=ds;
	delay_us(20); 
	ds=1;
	_nop_();
	return (i);
}

void write_byte(uchar dat)//DS18B20дʱ��
{
	uchar i;
	for(i=0;i<8;i++)
	{
		  ds= 0;
		  _nop_();
		  ds= dat & 0x01;
		  delay_us(10);//76.95us
		  ds= 1;
		  _nop_();
		  dat>>=1;
	}
}
uchar read_byte()//DS18B20��ʱ��
{
	uchar i, j, dat;
	for(i=0;i<8;i++)
	{
	  ds= 0;
	  _nop_();_nop_();//������ʱ��
	  ds= 1;
	  _nop_();_nop_();//�ͷ�����
	  j=ds;
	  delay_us(10);//76.95us
	  ds=1;
	  _nop_();_nop_();
	  dat=(j<<7)|(dat>>1); 
	}
	return (dat);
}


uint ReadTemperature(void)
{
	uint i;
	uchar L,M;
  	ds_init();//��ʼ��DS18B20
  	write_byte(0xcc);//������ԾROMָ��
  	write_byte(0x44);//�����¶�ת��ָ��
  	ds_init();//��ʼ��DS18B20
  	write_byte(0xcc);//������ԾROMָ��
  	write_byte(0xbe);//��ȡDS18B20�ݴ���ֵ
  	L = read_byte();
  	M = read_byte();
  	i = M;
  	i <<= 8;
  	i |= L;      
  	i = i * 0.0625*10+0.5;//
  	return i;
}