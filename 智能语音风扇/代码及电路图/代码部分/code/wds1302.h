/*------------------------------------
����:DS1302Driver
��д:YuMin_Dong
ʱ��:2013.01.20
------------------------------------*/
#ifndef _DS1302_H_
#define _DS1302_H_

/*------------------------------------
			I/Oλ����
------------------------------------*/
sbit DS_RST=P2^4;
sbit DS_SCL=P2^6;
sbit DS_IO=P2^5;

/*------------------------------------
			�궨��
------------------------------------*/
#define DS_RST_CLR DS_RST=0
#define DS_SCL_CLR DS_SCL=0
#define DS_IO_CLR  DS_IO=0
#define DS_IO_SET DS_IO=1
#define DS_SCL_SET DS_SCL=1
#define DS_RST_SET DS_RST=1

#define SECOND_ADDRESS 0x80		//�� �Ĵ��ַ
#define MINUTE_ADDRESS 0x82		//�� �Ĵ��ַ
#define HOUR_ADDRESS 0x84		//ʱ �Ĵ��ַ
#define DATE_ADDRESS 0x86		//�� �Ĵ��ַ
#define MONTH_ADDRESS 0x88		//�� �Ĵ��ַ
#define WEEK_ADDRESS 0x8a		//�� �Ĵ��ַ
#define YEAR_ADDRESS 0x8c		//�� �Ĵ��ַ
#define CONTROL_ADDRESS 0x8e	//���ƼĴ�����ַ

/*------------------------------------
			ȫ�ֱ�������
------------------------------------*/
UINT8 Year,Month,Date,Hour,Minute,Second,Week;	//������ʱ�����ܵ��ݴ�
bit Set_Hour;				//Ϊ1��ʾ����ʱ��,Ϊ0��ʾ���÷���,����������ʱ���ʱ��ͣ������
UINT8 Set_Date;				//����������ѡ���ʶ
bit Menu0;					//һ���Ӳ˵���ʱλ

UINT8 Set_Bell;
UINT8 Bell_Hour,Bell_Minute;

code UINT8 WEEK_TAB[12]={0,3,3,6,1,4,6,2,5,0,3,5};	//���ڲ���

/*------------------------------------
			Write����
------------------------------------*/
void DS1302_WriteByte(UINT8 address,UINT8 byte)
{
UINT8 i;
address=address&0xfe;	//R/W����λ�͵�λд
DS_RST_SET;
	for(i=0;i<8;i++)		//���͵�ַ,�ҼĴ���
	{
		if(address&0x01)
		DS_IO_SET;
		else DS_IO_CLR;
		address>>=1;
		DS_SCL_SET;
		DS_SCL_CLR;
	}
	for(i=0;i<8;i++)		//�������ݵ�Ѱ�ҵ��ļĴ���
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
			Read����
------------------------------------*/
UINT8 DS1302_ReadByte(UINT8 address)
{
UINT8 i,byte;
address=address|0x01;
DS_RST_SET;
	for(i=0;i<8;i++)		//���͵�ַ,�ҵ��Ĵ���
	{
		if(address&0x01)
		DS_IO_SET;
		else DS_IO_CLR;
		address>>=1;
		DS_SCL_SET;
		DS_SCL_CLR;
	}
	for(i=0;i<8;i++)		//���ҵ��ļĴ����ж�ȡ����
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
			��ȡʱ��
------------------------------------*/
void DS1302_ReadTime()
{
Hour=DS1302_ReadByte(HOUR_ADDRESS);
Minute=DS1302_ReadByte(MINUTE_ADDRESS);
Second=DS1302_ReadByte(SECOND_ADDRESS)&0x7f;
}

/*------------------------------------
			д��ʱ��
------------------------------------*/
void DS1302_WriteTime()
{
DS1302_WriteByte(CONTROL_ADDRESS,0X00);		//�ر�д����
DS1302_WriteByte(HOUR_ADDRESS,Hour);
DS1302_WriteByte(MINUTE_ADDRESS,Minute);	
DS1302_WriteByte(SECOND_ADDRESS,Second&0x7f);//������
DS1302_WriteByte(CONTROL_ADDRESS,0x80);		//��д����
}

/*------------------------------------
			��ʾʱ��
------------------------------------*/
void Display_Time()
{
LCD_WriteCommand(0x34);				//16x32��ʱ����ʾ,�ȿ�ʼ����ָ��ģʽ
LCD_WriteCommand(0x36);				//������ͼģʽ

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
			��������������
------------------------------------*/
void Display_Bell()
{
LCD_WriteCommand(0x34);				//16x32��ʱ����ʾ,�ȿ�ʼ����ָ��ģʽ
LCD_WriteCommand(0x36);				//������ͼģʽ

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
			��ȡ����
------------------------------------*/
void DS1302_ReadDate()
{
Year=DS1302_ReadByte(YEAR_ADDRESS);	
Month=DS1302_ReadByte(MONTH_ADDRESS);
Date=DS1302_ReadByte(DATE_ADDRESS);		
}

/*------------------------------------
			д������
------------------------------------*/
void DS1302_WriteDate()
{
DS1302_WriteByte(CONTROL_ADDRESS,0X00);		//�ر�д����
DS1302_WriteByte(DATE_ADDRESS,Date);
DS1302_WriteByte(MONTH_ADDRESS,Month);	
DS1302_WriteByte(YEAR_ADDRESS,Year);//������
DS1302_WriteByte(CONTROL_ADDRESS,0x80);		//��д����
}

/*------------------------------------
		����ʱ������������
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
			����ת��
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
			��ʾ����
------------------------------------*/
void Display_Date()
{
//if(Set_Year&&!Set_Month)
//Year=~Year;
LCD_WriteCommand(0x30);
LCD_WriteCommand(0x06);
						//��ʾ���ڸ�ʽ20XX-XX-XX
LCD_WriteCommand(0x80); 
		//��1302��������Ϣ
LCD_SendString("20");
LCD_WriteData(Year/16|0x30);				//�������Ϣ
LCD_WriteData(Year%16|0x30);

LCD_WriteData('-');							//�ָ���

	//��1302��������Ϣ
LCD_WriteData(Month/16|0x30);				//LCD�������Ϣ
LCD_WriteData(Month%16|0x30);

LCD_WriteData('-');							//�ָ���

		//��1302��������Ϣ
LCD_WriteData(Date/16|0x30);				//LCD�������Ϣ
LCD_WriteData(Date%16|0x30);
}

/*------------------------------------
			���㲢��ʾ����
------------------------------------*/
void Display_Week()
{
LCD_WriteCommand(0x86);
//Week=DS1302_ReadByte(WEEK_ADDRESS);
Week=Conver_Week(0,Year,Month,Date);
LCD_SendString("��");
	switch(Week)
	{
	case 0: LCD_SendString("��");
			break;
	case 1:	LCD_SendString("һ");
			break;
	case 2: LCD_SendString("��"); 
			break;
	case 3:	LCD_WriteData(0xc8);		//�ֿ���λ���ó�"��"����ʾ��.
			LCD_WriteData(0xfd);
			break;
	case 4: LCD_SendString("��");
			break;
	case 5: LCD_SendString("��");
			break;
	case 6: LCD_SendString("��");
			break;
	default:return ;
	}
}

/*------------------------------------
			��ʼ��
------------------------------------*/
void DS1302_Init()			//��ʼ��DS1302
{
DS_SCL_CLR;
DS_RST_CLR;
//DS1302_WriteByte(CONTROL_ADDRESS,0X00);
//DS1302_WriteByte(HOUR_ADDRESS,0x16);
//DS1302_WriteByte(CONTROL_ADDRESS,0x80);

}
#endif