/***************************飞音云电子****************************
**  工程名称：YS-V0.7语音识别模块驱动程序
**	CPU: STC11L08XE
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-V0.7语音识别开发板
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2013.9.13
**  说明：口令模式+IO控制： 即每次识别时都需要说“小杰”这个口令 ，才能够进行下一级的识别
/***************************飞音云电子******************************/
#include "config.h"
#include<math.h> 
//#include "stdio.h"


sbit en1=P3^4; /* L298的Enable A */ 

/************************************************************************************/
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:		表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:		表示一次识别流程中LD3320芯片内部出现不正确的状态
/***********************************************************************************/
uint8 idata nAsrStatus=0;	
void MCU_init(); 
void ProcessInt0(); //识别处理函数
void User_handle(uint8 dat);//用户执行操作函数
void Led_test(void);//单片机工作指示
uint8_t G0_flag=DISABLE;//运行标志，ENABLE:运行。DISABLE:禁止运行 




extern uchar Fanflag;//风扇标志位

uchar t=0; /* 中断计数器 */ 
uchar m1=0; /* 电机1速度值 */ 
uchar tmp1; /* 电机当前速度值 */ 



/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
int  main(void)	  	
{
	uint8 idata nAsrRes;
	en1=0;
	
	Led_test();	
	MCU_init();
	LD_Reset();
	UartIni(); /*串口初始化*/
		
	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR 
	//set_time();
	lcd_init();//初始化LCD12864
	SYN_FrameInfo(0,"系统启动中");
	display_12864(1,1,"智能语音风扇");
	display_12864(2,1,"西北民族大学");
	display_12864(3,1,"王昭武邓天金");
	display_12864(4,2,"Welcome!");
	
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	lcd_init();//初始化LCD12864
	ClearScreen();lcd_wcmd(0x01);//清屏
	SYN_FrameInfo(0,"初始化完成");	
	Delay200ms();
	while(1)  
	{ 
		
		read_time();
		DisplayTime();//显示时间及温度
		
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:
			break;
			case LD_ASR_NONE:
			{
				nAsrStatus=LD_ASR_RUNING;
				if (RunASR()==0)	/*	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算*/
				{
					nAsrStatus = LD_ASR_ERROR;
				
				}
				break;
			}  
			case LD_ASR_FOUNDOK: /*	一次ASR识别流程结束，去取ASR识别结果*/
			{				
				nAsrRes = LD_GetResult();		/*获取结果*/
				User_handle(nAsrRes);//用户执行函数 
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
* 名    称： 	 LED灯测试
* 功    能： 单片机是否工作指示
* 入口参数： 无 
* 出口参数：无
* 说    明： 					 
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
* 名    称： void MCU_init()
* 功    能： 单片机初始化
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void MCU_init()
{
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;

//	P1M0=0xff;	//P1端口设置为推挽输出功能，即提高IO口驱动能力，从驱动继电器模块工作
//	P1M1=0x00;
	
	TMOD=0x02; /* 设定T0的工作模式为2 ,8位自动重置定时器，每过几个机器周期，计数器加一*/ 
	TH0=0x9B; /* 装入定时器的初值，计数100次溢出 */ 
	TL0=0x9B; /*装入时间常数*/
	TR0=0;
	ET0=1;
	en1=0;
	

	LD_MODE = 0;		//	设置MD管脚为低，并行模式读写
	IE0=1;
	EX0=1;
	EA=1;
}


void timer0() interrupt 1 /* T0中断服务程序 */ 	
{ 
	if(t==0) /* 1个PWM周期完成后才会接受新数值 */ 
	{ 
		tmp1=m1; 
	} 
	if(t<tmp1) en1=1; else en1=0; /* 产生电机1的PWM信号 */ 
	t++; 
	if(t>=100) t=0; /* 1个PWM信号由100次中断产生 */ 
}

/***********************************************************
* 名    称：	延时函数
* 功    能：
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 


/***********************************************************
* 名    称： 中断处理函数
* 功    能：
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void ExtInt0Handler(void) interrupt 0  
{ 	
	ProcessInt0();				/*	LD3320 送出中断信号，包括ASR和播放MP3的中断，需要在中断处理函数中分别处理*/
	read_time();
}
/***********************************************************
* 名    称：用户执行函数 
* 功    能：识别成功后，执行动作可在此进行修改 
* 入口参数： 无 
* 出口参数：无
* 说    明： 通过控制PAx端口的高低电平，从而控制外部继电器的通断					 
**********************************************************/
void 	User_handle(uint8 dat)
{

		 if(0==dat)
		 {
		  SYN_FrameInfo(0,"风扇已打开");
		  Delay200ms();
		  G0_flag=ENABLE;
		  LED=0;
		  Fanflag=1;
		 }
		 else if(ENABLE==G0_flag)
		 {	
		 	//	G0_flag=DISABLE;
			LED=1;
			 switch(dat)		   /*对结果执行相关操作,客户修改*/
			  {
				  case CODE_DMCS:			/*命令“高速”*/
					SYN_FrameInfo(0,"高速命令识别成功");
				    Delay200ms();
					Fanflag=2;TR0=1;m1=100;
				  	
												   break;
					
					case CODE_KFBYZ:	 /*命令“”*/
					
													 break;
					case CODE_KD:		/*命令“低速”*/	
					SYN_FrameInfo(0,"低速命令识别成功");
					Delay200ms();
					Fanflag=3;TR0=1;m1=50;
					
					
													break;
					case CODE_GD:		/*命令“关闭”*/
					SYN_FrameInfo(0,"关闭命令识别成功");
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
					SYN_FrameInfo(0,"系统忙碌中");
					Delay200ms();
					
						
													break;
				}	
			}	
			else 	
			{
				//PrintCom("请说出一级口令\r\n"); //串口输出提示信息（可删除）	
			}  
}	
 
