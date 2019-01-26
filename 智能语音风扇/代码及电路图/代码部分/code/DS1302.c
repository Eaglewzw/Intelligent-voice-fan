#include "DS1302.h"

uchar time[]={18,1,10,1,10,19,12};//年周月日时分秒
uchar write_add[]={0x8c,0x8a,0x88,0x86,0x84,0x82,0x80};
uchar read_add[]={0x8d,0x8b,0x89,0x87,0x85,0x83,0x81};
uchar sfm[14];



void write_ds1302_byte(uchar date)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		sclk=0;
		io=date&0x01;
		date>>=1;
		sclk=1;					
	}			
}
void write_ds1302(uchar add,uchar date)
{
	rst=0;
	_nop_();
	_nop_();
	_nop_();
	sclk=0;
	_nop_();
	_nop_();
	_nop_();
	rst=1;
	_nop_();
	_nop_();
	_nop_();
	write_ds1302_byte(add);
	write_ds1302_byte(date);
	rst=0;
	_nop_();
	_nop_();
	_nop_();
	io=1;
	sclk=1;
			
}
uchar read_ds1302(uchar add)
{
	uchar i,value;
	rst=0;
	_nop_();
	_nop_();
	_nop_();
	sclk=0;
	_nop_();
	_nop_();
	_nop_();
	rst=1;
	_nop_();
	_nop_();
	_nop_();
	write_ds1302_byte(add);
	for(i=0;i<8;i++)
	{
		io=1;
		value=value>>1;
		sclk=0;
		if(io==1)
		value=value|0x80;
		sclk=1;
		io=0;
	}
	rst=0;
	_nop_();
	_nop_();
	_nop_();
	sclk=0;
	_nop_();
	_nop_();
	_nop_();
	sclk=1;
	io=1;	
	return value;
}
void set_time()			//把时间写进去
{
	uchar i,j;
	for(i=0;i<7;i++)		//转BCD码
	{
		j=time[i]/10;
		time[i]=time[i]%10;
		time[i]=time[i]+j*16;
	}
	write_ds1302(0x8e,0x00); //去除写保护
	for(i=0;i<7;i++)
	{
		write_ds1302(write_add[i],time[i]);
	}
	write_ds1302(0x8e,0x80); //开写保护
}
 void read_time()	  //读时间
{
	uchar i;
	for(i=0;i<7;i++)
	time[i]=read_ds1302(read_add[i]);	
	
	
	//BCM转十进制数
	sfm[13]=time[6]%16;		  //秒
	sfm[12]=time[6]/16;

	sfm[11]=time[5]%16;		  //分
	sfm[10]=time[5]/16;

	sfm[9]=time[4]%16;		  //时
	sfm[8]=time[4]/16;

	sfm[7]=time[3]%16;		//日
	sfm[6]=time[3]/16;

	sfm[5]=time[2]%16;		//月
	sfm[4]=time[2]/16;

	sfm[3]=time[1]%16;		//星期
	sfm[2]=time[1]/16;

	sfm[1]=time[0]%16;		  //年
	sfm[0]=time[0]/16;	 
}



uchar Conver_week(uchar cyear,uchar cmonth,uchar cday)
{
	uchar temp,week;
	cyear=100+cyear;
	temp=cyear+(cyear/4); //如果为21世纪,年份数加100
	temp=temp%7;    //为节省资源,先进行一次取余,避免数大于0xff,避免使用整型数据
	temp=temp+cday+table_week[cmonth-1]; //日期+年份+所过闰年数+月较正数之和除7
	if(cyear%4==0&&cmonth<3)
		{temp=temp-1;}//如果是在闰年又不到3月份上述之和要减一天再除7 
	week=temp%7;
	return week;
}
