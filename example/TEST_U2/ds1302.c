#include "ds1302.h"


code unsigned char w_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //秒分时日月周年
code unsigned char r_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d}; 

RTC devRTC;

//
RTC *GetRTCStruct(void)
{
	return &devRTC;
}


/*
*/
void Write_Ds1302_Byte(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
   	_nop_();  
 	Write_Ds1302_Byte(address);	
 	Write_Ds1302_Byte(dat);		
 	RST=0; 
}

unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;
	_nop_();
 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;
	_nop_();
 	RST=0;
	SCK=0;
	_nop_();
	SCK=1;
	_nop_();
	SDA=0;
	_nop_();
	SDA=1;
	_nop_();
	return (temp);			
}

/*
*/
void ReadRTC(void)
{
 	unsigned char i, *p;
	unsigned char tmp[3];
	
 	p = (unsigned char *)r_rtc_address; 	//地址传递
 	
 	for(i=0;i<3;i++){
	  	tmp[i]=Read_Ds1302(*p);
	  	p++;
 	}
	
	devRTC.rtc[0] = (tmp[2] >> 4);
	devRTC.rtc[1] = (tmp[2] & 0x0F);
	
	devRTC.rtc[2] = (tmp[1] >> 4);
	devRTC.rtc[3] = (tmp[1] & 0x0F);
	
	devRTC.rtc[4] = (tmp[0] >> 4);
	devRTC.rtc[5] = (tmp[0] & 0x0F);
}

/*
*/
void SetRTC(void)
{
 	Write_Ds1302(0x8E,0X00);
	
	Write_Ds1302(w_rtc_address[0], (devRTC.set[4]<<4) | (devRTC.set[5]));
	Write_Ds1302(w_rtc_address[1], (devRTC.set[2]<<4) | (devRTC.set[3]));
	Write_Ds1302(w_rtc_address[2], (devRTC.set[0]<<4) | (devRTC.set[1]));
	
	Write_Ds1302(0x8E,0x80);
}

//
void InitRTCStruct(void)
{
	devRTC.set[0] = 2;
	devRTC.set[1] = 3;
	devRTC.set[2] = 5;
	devRTC.set[3] = 9;
	devRTC.set[4] = 5;
	devRTC.set[5] = 0;

	devRTC.Read = ReadRTC;
	devRTC.Set = SetRTC;
	
}



