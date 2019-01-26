/***************************�����Ƶ���****************************
**  �������ƣ�YS-V0.7����ʶ��ģ����������
**	CPU: STC11L08XE
**	����22.1184MHZ
**	�����ʣ�9600 bit/S
**	���ײ�Ʒ��Ϣ��YS-V0.7����ʶ�𿪷���
**                http://yuesheng001.taobao.com
**  ���ߣ�zdings
**  ��ϵ��751956552@qq.com
**  �޸����ڣ�2013.9.13
**  ˵��������ģʽ+IO���ƣ� ��ÿ��ʶ��ʱ����Ҫ˵��С�ܡ�������� �����ܹ�������һ����ʶ��
/***************************�����Ƶ���******************************/
#include "config.h"
#include<math.h> 
//#include "stdio.h"


sbit en1=P3^4; /* L298��Enable A */ 

/************************************************************************************/
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_NONE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUNDOK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUNDZERO:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
/***********************************************************************************/
uint8 idata nAsrStatus=0;	
void MCU_init(); 
void ProcessInt0(); //ʶ������
void User_handle(uint8 dat);//�û�ִ�в�������
void Led_test(void);//��Ƭ������ָʾ
uint8_t G0_flag=DISABLE;//���б�־��ENABLE:���С�DISABLE:��ֹ���� 




extern uchar Fanflag;//���ȱ�־λ

uchar t=0; /* �жϼ����� */ 
uchar m1=0; /* ���1�ٶ�ֵ */ 
uchar tmp1; /* �����ǰ�ٶ�ֵ */ 



/***********************************************************
* ��    �ƣ� void  main(void)
* ��    �ܣ� ������	�������
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
int  main(void)	  	
{
	uint8 idata nAsrRes;
	en1=0;
	
	Led_test();	
	MCU_init();
	LD_Reset();
	UartIni(); /*���ڳ�ʼ��*/
		
	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR 
	//set_time();
	lcd_init();//��ʼ��LCD12864
	SYN_FrameInfo(0,"ϵͳ������");
	display_12864(1,1,"������������");
	display_12864(2,1,"���������ѧ");
	display_12864(3,1,"����������");
	display_12864(4,2,"Welcome!");
	
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	lcd_init();//��ʼ��LCD12864
	ClearScreen();lcd_wcmd(0x01);//����
	SYN_FrameInfo(0,"��ʼ�����");	
	Delay200ms();
	while(1)  
	{ 
		
		read_time();
		DisplayTime();//��ʾʱ�估�¶�
		
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:
			break;
			case LD_ASR_NONE:
			{
				nAsrStatus=LD_ASR_RUNING;
				if (RunASR()==0)	/*	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����*/
				{
					nAsrStatus = LD_ASR_ERROR;
				
				}
				break;
			}  
			case LD_ASR_FOUNDOK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
			{				
				nAsrRes = LD_GetResult();		/*��ȡ���*/
				User_handle(nAsrRes);//�û�ִ�к��� 
				nAsrStatus = LD_ASR_NONE;
				
				break;
			}
			case LD_ASR_FOUNDZERO:
			default:
			{
				nAsrStatus = LD_ASR_NONE;
				
				break;
			}
		}	// switch		 		
	}// while  */

  }
/***********************************************************
* ��    �ƣ� 	 LED�Ʋ���
* ��    �ܣ� ��Ƭ���Ƿ���ָʾ
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
**********************************************************/
void Led_test(void)
{
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
}
/***********************************************************
* ��    �ƣ� void MCU_init()
* ��    �ܣ� ��Ƭ����ʼ��
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void MCU_init()
{
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;

//	P1M0=0xff;	//P1�˿�����Ϊ����������ܣ������IO�������������������̵���ģ�鹤��
//	P1M1=0x00;
	
	TMOD=0x02; /* �趨T0�Ĺ���ģʽΪ2 ,8λ�Զ����ö�ʱ����ÿ�������������ڣ���������һ*/ 
	TH0=0x9B; /* װ�붨ʱ���ĳ�ֵ������100����� */ 
	TL0=0x9B; /*װ��ʱ�䳣��*/
	TR0=0;
	ET0=1;
	en1=0;
	

	LD_MODE = 0;		//	����MD�ܽ�Ϊ�ͣ�����ģʽ��д
	IE0=1;
	EX0=1;
	EA=1;
}


void timer0() interrupt 1 /* T0�жϷ������ */ 	
{ 
	if(t==0) /* 1��PWM������ɺ�Ż��������ֵ */ 
	{ 
		tmp1=m1; 
	} 
	if(t<tmp1) en1=1; else en1=0; /* �������1��PWM�ź� */ 
	t++; 
	if(t>=100) t=0; /* 1��PWM�ź���100���жϲ��� */ 
}

/***********************************************************
* ��    �ƣ�	��ʱ����
* ��    �ܣ�
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 


/***********************************************************
* ��    �ƣ� �жϴ�����
* ��    �ܣ�
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void ExtInt0Handler(void) interrupt 0  
{ 	
	ProcessInt0();				/*	LD3320 �ͳ��ж��źţ�����ASR�Ͳ���MP3���жϣ���Ҫ���жϴ������зֱ���*/
	read_time();
}
/***********************************************************
* ��    �ƣ��û�ִ�к��� 
* ��    �ܣ�ʶ��ɹ���ִ�ж������ڴ˽����޸� 
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� ͨ������PAx�˿ڵĸߵ͵�ƽ���Ӷ������ⲿ�̵�����ͨ��					 
**********************************************************/
void 	User_handle(uint8 dat)
{

		 if(0==dat)
		 {
		  SYN_FrameInfo(0,"�����Ѵ�");
		  Delay200ms();
		  G0_flag=ENABLE;
		  LED=0;
		  Fanflag=1;
		 }
		 else if(ENABLE==G0_flag)
		 {	
		 	//	G0_flag=DISABLE;
			LED=1;
			 switch(dat)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
			  {
				  case CODE_DMCS:			/*������١�*/
					SYN_FrameInfo(0,"��������ʶ��ɹ�");
				    Delay200ms();
					Fanflag=2;TR0=1;m1=100;
				  	
												   break;
					
					case CODE_KFBYZ:	 /*�����*/
					
													 break;
					case CODE_KD:		/*������١�*/	
					SYN_FrameInfo(0,"��������ʶ��ɹ�");
					Delay200ms();
					Fanflag=3;TR0=1;m1=50;
					
					
													break;
					case CODE_GD:		/*����رա�*/
					SYN_FrameInfo(0,"�ر�����ʶ��ɹ�");
					Delay200ms();
					Fanflag=4;TR0=0;en1=0;
					
													break;
					case CODE_BJ:			
					
													break;
					case CODE_SH:		
					
													break;
					case CODE_GZ:							
					
													break;																											
					default:
					Fanflag=5;
					SYN_FrameInfo(0,"ϵͳæµ��");
					Delay200ms();
					
						
													break;
				}	
			}	
			else 	
			{
				//PrintCom("��˵��һ������\r\n"); //���������ʾ��Ϣ����ɾ����	
			}  
}	
 
