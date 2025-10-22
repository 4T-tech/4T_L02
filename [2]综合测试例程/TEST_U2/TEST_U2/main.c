#include "stc12.h"
#include <intrins.h>
#include <string.h>
#include "ds18b20.h"
#include "at24c02.h"
#include "ds1302.h"
#include "pcf8591.h"

/*
���������	 	��
��������     	��
RTC					 	��
8951					��
*/


#define uint8_t  unsigned char
#define uint16_t unsigned int
	
sbit S1 = P1^0;
sbit S2 = P1^1;
sbit S3 = P1^2;
sbit S4 = P1^3;

uint8_t rom_data[8];

uint8_t wake_times;

uint8_t seg_number[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xbf};     //0��9��A��F�Ĺ�������ʾ��
uint8_t seg_com[9] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xff}; //������
uint8_t seg_data[8] = {0};

uint8_t adc_data;

uint8_t key_status = 5;
uint8_t key_status_old;

void uart_init();
void uart_send_data(uint8_t *tdata,uint8_t length);
void rom_code_c();
void delay_ms(uint16_t times);
void seg_dispaly(uint8_t *tdata,uint8_t com_control);
void seg_serial();
uint8_t at24c02_init();
void display_waketimes(uint8_t wake_times);
void display_ds1302(void);
void pcf8591_dac(uint8_t dac);
void pcf8591_adc(uint8_t channel);
void keyboart_check(void);
void task(uint8_t i);
	

void main()
{
	uint8_t i;
	
	uart_init();
	
	/* get wake times */
	wake_times = at24c02_init();
	
	
	/* serial seg display*/
	seg_serial();

	/* rtc */
	InitRTCStruct();
	GetRTCStruct()->Set();
	
	/* 8591 */
	InitPCF8591();
	
	while(1) 
	{
		keyboart_check();
		task(i++);
		
		
		
	}
}

void uart_init() 
{	
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x20;		//���ö�ʱ��ģʽ
	TL1 = 0xFD;			//���ö�ʱ��ʼֵ
	TH1 = 0xFD;			//���ö�ʱ����ֵ
	ET1 = 0;			//��ֹ��ʱ���ж�
	TR1 = 1;			//��ʱ��1��ʼ��ʱ
}

void uart_send_data(uint8_t *tdata,uint8_t length)
{
	uint8_t i;
	for (i=0;i<length;i++)
	{
		SBUF=*(tdata+i); 			// ���͵�i������
		while(TI==0);     // ��ѯ�ȴ������Ƿ����
		TI=0;	      // ������ɣ�TI�������0
	}
}


void delay_ms(uint16_t times)	//@11.0592MHz
{
	uint8_t i, j;
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
/*
tdata  seg_data 
com_control   0x1111_1111  all on
              0x0000_1111  half on
eg.
		output	1_95_533
		input   tdata[8] = {1,x,9,5,x,5,3,3}
						com_control   = 0x10110111;
*/
void seg_dispaly(uint8_t *tdata,uint8_t com_control)
{
	uint8_t i; 
	
	for(i=0;i<8;i++) //����com
	{
		P0=0xff;   		//����ʾ
		P20 = 0;			//дcom
		if((com_control>>(7-i)& 0x01) == 1)
		{
			P0=~seg_com[i]; //λѡ����λ���ƿ�P0�� 
		}
		else
		{
			P0=~seg_com[8];
		}
		P20 = 1;
		P0=seg_number[*(tdata+i)];   							 	//��ʾ��������P1��
		delay_ms(1);	
	}
	
}

void seg_serial()
{
	uint8_t i;
	P23 = 0;
	SCON=0x00;					//�贮�пڷ�ʽ0
	P23 = 1;
	for (i=0;i<0x10;i++)		
	{ 
		SBUF=seg_number[i];			//����ʾ����
		TI=0;
		while(!TI);			//�ȴ��������
		delay_ms(400);
	}
	SBUF=seg_number[16];			//����ʾ����
	TI=0;
	while(!TI);			//�ȴ��������
	SBUF=seg_number[16];			//����ʾ����
	TI=0;
	while(!TI);			//�ȴ��������
}

uint8_t at24c02_init()
{
	/*���������ʾ����������ģ��i2c�����ļ�*/
	unsigned char times;
	times = read_AT24C02(254);
	delay_ms(100);	
	if(times < 255 )
	{
			times=times+1;
			write_AT24C02(254,times);
	}
	else
	{
		times = 1;
		write_AT24C02(254,times);
		
	}
	return times;
	
}

void display_waketimes(uint8_t wake_times)
{
	seg_data[5] = wake_times/100;
	seg_data[6] = wake_times%100/10;
	seg_data[7] = wake_times%100%10;
	if(seg_data[5]>0) seg_dispaly(seg_data,0x07);
	else if(seg_data[5]==0 && seg_data[6]>0) seg_dispaly(seg_data,0x03);
	else if(seg_data[5]==0 && seg_data[6]==0) seg_dispaly(seg_data,0x01);
}

void display_ds1302(void)
{
	GetRTCStruct()->Read();
	seg_data[0] = GetRTCStruct()->rtc[0];
	seg_data[1] = GetRTCStruct()->rtc[1];
	seg_data[2] = 16;
	seg_data[3] = GetRTCStruct()->rtc[2];
	seg_data[4] = GetRTCStruct()->rtc[3];
	seg_data[5] = 16;
	seg_data[6] = GetRTCStruct()->rtc[4];
	seg_data[7] = GetRTCStruct()->rtc[5];

	seg_dispaly(seg_data,0xff);
}

void pcf8591_dac(uint8_t dac)
{
	SetDAC(dac);
	if(seg_data[5]>0) seg_dispaly(seg_data,0x07);
	else if(seg_data[5]==0 && seg_data[6]>0) seg_dispaly(seg_data,0x03);
	else if(seg_data[5]==0 && seg_data[6]==0) seg_dispaly(seg_data,0x01);
	
	seg_data[5] = dac/100;
	seg_data[6] = dac%100/10;
	seg_data[7] = dac%100%10;
}
	
void pcf8591_adc(uint8_t channel)
{
	uint8_t i;
	for(i=0;i<5;i++)
	{
		if(seg_data[5]>0) seg_dispaly(seg_data,0x07);
		else if(seg_data[5]==0 && seg_data[6]>0) seg_dispaly(seg_data,0x03);
		else if(seg_data[5]==0 && seg_data[6]==0) seg_dispaly(seg_data,0x01);
	}
	adc_data = GetADC(channel);
	seg_data[5] = adc_data/100;
	seg_data[6] = adc_data%100/10;
	seg_data[7] = adc_data%100%10;

}
void keyboart_check(void)
{
	 if(S1 == 0)
	 {
		 if(key_status_old == 1)
		 {
			 key_status = 1;
		 }
		 else
		 {
			 key_status_old = 1;
		 }
		 
	 }
	 else if(S2 == 0)
	 {
		 if(key_status_old == 2)
		 {
			 key_status = 2;
		 }
		 else
		 {
			 key_status_old = 2;
		 }
		 
	 }
	 
	 else if(S3 == 0)
	 {
		 if(key_status_old == 3)
		 {
			 key_status = 3;
		 }
		 else
		 {
			 key_status_old = 3;
		 }
		 
	 }
	 else if(S4 == 0)
	 {
		 if(key_status_old == 4)
		 {
			 key_status = 4;
		 }
		 else
		 {
			 key_status_old = 4;
		 }
	 }
}

void task(uint8_t i)
{
	switch(key_status)
	{
		case 5:
			display_waketimes(wake_times);
			break;
		case 2:
			display_ds1302();
			break;
		case 3:
			pcf8591_dac(i);
			break;
		case 1:
			display_waketimes(wake_times);
		case 4:
			pcf8591_adc(1);
			break;
	}

}