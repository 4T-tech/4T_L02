#ifndef __DS1302_H
#define __DS1302_H

#include "stc12.h"
#include "intrins.h"

sbit SCK=P2^5;		
sbit SDA=P2^4;		
sbit RST = P2^6;   // DS1302复位

typedef struct{
	unsigned char rtc[6];
	unsigned char set[6];

	void (code *Read)(void);
	void (code *Set)(void);
}RTC;

void InitRTCStruct(void);
RTC *GetRTCStruct(void);


#endif
