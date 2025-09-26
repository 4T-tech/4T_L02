#include "stc12.h"
#include <intrins.h>
#include <string.h>
/*
1.led��Ȧ�ƣ������л�����һ�ֹ۲�һ��
2.��SW2��SW24�Ĳ������أ����ϵĻ��������͵��󶼻�����
*/
#define uint8_t  unsigned char
#define uint16_t unsigned int
	

/*��������*/
sbit SER = P3^5;
sbit SRCLK = P3^6;						//��λ�Ĵ�����ʱ������,�����յ�������ʱ��λ�Ĵ����ڲ������������;
sbit SRCLR = P3^7; 						//��λ����,�����Žӵ͵�ƽʱоƬ�ڲ����ݴ洢�Ĵ����������㸴λ.�粻ʹ��һ��Ӹߵ�ƽ;
sbit RCLK = P3^4;							//���ݴ洢�Ĵ���ʱ������,����⵽������ʱʹ�����ݴ洢�Ĵ���
/*4-16������*/
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

void delay(unsigned int i);   //��ʱ��������
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

//��������delay
//�������ܣ�ʵ�������ʱ
//��ʽ�������޷������ͱ���i�����ƿ�ѭ����ѭ������
//����ֵ����
void delay(unsigned int i) 	  //��ʱ����
{
  unsigned int k;
	for(k=0;k<i;k++);
}

/* ����74HC595���ʹ������� */
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

/* �������� */
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
