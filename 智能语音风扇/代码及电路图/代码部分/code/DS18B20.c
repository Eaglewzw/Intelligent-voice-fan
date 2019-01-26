#include"DS18B20.h"


void delay_us(uchar us)
{
	uint i=2;
	while(i--)
	while(us--); 
}

bit ds_init()//单总线初始化时序
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

void write_byte(uchar dat)//DS18B20写时序
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
uchar read_byte()//DS18B20读时序
{
	uchar i, j, dat;
	for(i=0;i<8;i++)
	{
	  ds= 0;
	  _nop_();_nop_();//产生读时序
	  ds= 1;
	  _nop_();_nop_();//释放总线
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
  	ds_init();//初始化DS18B20
  	write_byte(0xcc);//发送跳跃ROM指令
  	write_byte(0x44);//发送温度转换指令
  	ds_init();//初始化DS18B20
  	write_byte(0xcc);//发送跳跃ROM指令
  	write_byte(0xbe);//读取DS18B20暂存器值
  	L = read_byte();
  	M = read_byte();
  	i = M;
  	i <<= 8;
  	i |= L;      
  	i = i * 0.0625*10+0.5;//
  	return i;
}