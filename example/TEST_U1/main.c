#include "stc12.h"
#include <intrins.h>
#include <string.h>
/*
1.led三圈灯，可以切换成另一种观察一下
2.看SW2和SW24的拨动开关，往上的话蜂鸣器和点阵都会在用
*/
#define uint8_t  unsigned char
#define uint16_t unsigned int
	

/*串行数据*/
sbit SER = P3^5;
sbit SRCLK = P3^6;						//移位寄存器的时钟输入,当接收到上升沿时移位寄存器内部数据整体后移;
sbit SRCLR = P3^7; 						//复位引脚,当引脚接低电平时芯片内部数据存储寄存器数据清零复位.如不使用一般接高电平;
sbit RCLK = P3^4;							//数据存储寄存器时钟输入,当检测到上升沿时使能数据存储寄存器
/*4-16译码器*/
sbit A0 = P2^6;
sbit A1 = P2^7;
sbit A2 = P3^2;
sbit A3 = P3^3;
sbit E0 = P2^5;

sbit S_L=P2^0;
sbit S_R=P2^1;
sbit beep=P2^3;


unsigned char code led[10][8] = { 
																	{0x18,0x24,0x24,0x24,0x24,0x24,0x24,0x18},//0
																	{0x00,0x18,0x1c,0x18,0x18,0x18,0x18,0x18},//1
																	{0x00,0x1e,0x30,0x30,0x1c,0x06,0x06,0x3e},//2
																	{0x00,0x1e,0x30,0x30,0x1c,0x30,0x30,0x1e},//3
																	{0x00,0x30,0x38,0x34,0x32,0x3e,0x30,0x30},//4
																	{0x00,0x1e,0x02,0x1e,0x30,0x30,0x30,0x1e},//5
																	{0x00,0x1c,0x06,0x1e,0x36,0x36,0x36,0x1c},//6
																	{0x00,0x3f,0x30,0x18,0x18,0x0c,0x0c,0x0c},//7
																	{0x00,0x1c,0x36,0x36,0x1c,0x36,0x36,0x1c},//8
																	{0x00,0x1c,0x36,0x36,0x36,0x3c,0x30,0x1c}	//9
																};

void delay(unsigned int i);   //延时函数声明
void send_serial_data(unsigned int tdata);
void set_row(unsigned char row);
																
																
void led_running();

void main()
{
	uint8_t i,j,p;

	led_running();

	beep = 0;	


	while(1)
	{		
		for(j=0;j<10;j++)
		{
			if(S_R == 1 )
			{

				for(p=0;p<10;p++)
				for(i=0;i<8;i++)
				{		
					E0 = 1;
					send_serial_data(led[j][i]);
					E0 = 0;
					set_row(i);
					if(S_L == 1)
					{
						beep = 0;
						delay(500);
						beep = 1;
					}
					else
					{
						delay(500);
						beep = 1;
					}
				}	
			}
			else
			{
				E0 = 1;
				if(S_L == 1)
				{
					beep = 0;
					delay(500);
					beep = 1;
				}
				else
				{
					beep = 1;
				}
				delay(300);
			}
		}
	}



}



void delay_ms(uint16_t times)	//@11.0592MHz
{
	unsigned char data i, j;
	uint16_t k;
	for(k=0;k<times;k++)
	{
		_nop_();
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
	}
}

void led_running()
{
	uint8_t i,j;
	uint8_t led_flag = 0xfe;
	for(j=0;j<3;j++)
	for(i=0;i<8;i++)
	{
		P0 = led_flag;
		led_flag =_crol_(led_flag,1);
		delay_ms(100);
	}
	
	P0 = 0xff;
	
	
}

//函数名：delay
//函数功能：实现软件延时
//形式参数：无符号整型变量i，控制空循环的循环次数
//返回值：无
void delay(unsigned int i) 	  //延时函数
{
  unsigned int k;
	for(k=0;k<i;k++);
}

/* 驱动74HC595发送串行数据 */
void send_serial_data(unsigned int tdata)
{
	unsigned char i;
	unsigned int serial_data;
	serial_data = ~tdata;
	for(i=0;i<8;i++)
	{

		SER = (serial_data>>i)&0x01;
		SRCLK = 0 ;
		delay(1);
		SRCLK = 1;
		delay(1);
		RCLK = 0;
		delay(1);
		RCLK = 1;
		delay(1);

	}
}

/* 设置行数 */
void set_row(unsigned char row)
{
	switch(row)
	{
		case 0 : A3 = 0; A2 = 0; A1 = 0 ;A0 = 0; break;
		case 1 : A3 = 0; A2 = 0; A1 = 0 ;A0 = 1; break;
		case 2 : A3 = 0; A2 = 0; A1 = 1 ;A0 = 0; break;
		case 3 : A3 = 0; A2 = 0; A1 = 1 ;A0 = 1; break;
		case 4 : A3 = 0; A2 = 1; A1 = 0 ;A0 = 0; break;
		case 5 : A3 = 0; A2 = 1; A1 = 0 ;A0 = 1; break;
		case 6 : A3 = 0; A2 = 1; A1 = 1 ;A0 = 0; break;
		case 7 : A3 = 0; A2 = 1; A1 = 1 ;A0 = 1; break;
//		case 8 : A3 = 1; A2 = 0; A1 = 0 ;A0 = 0; break;
//		case 9 : A3 = 1; A2 = 0; A1 = 0 ;A0 = 1; break;
//		case 10 : A3 = 1; A2 = 0; A1 = 1 ;A0 = 0; break;
//		case 11 : A3 = 1; A2 = 0; A1 = 1 ;A0 = 1; break;
//		case 12 : A3 = 1; A2 = 1; A1 = 0 ;A0 = 0; break;
//		case 13 : A3 = 1; A2 = 1; A1 = 0 ;A0 = 1; break;
//		case 14 : A3 = 1; A2 = 1; A1 = 1 ;A0 = 0; break;
//		case 15 : A3 = 1; A2 = 1; A1 = 1 ;A0 = 1; break;
		default : break;
	}
	
}
