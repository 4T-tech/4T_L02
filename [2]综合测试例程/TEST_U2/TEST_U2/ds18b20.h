#ifndef __DS18B20_H
#define __DS18B20_H

#include "stc12.h"

sbit DQ = P2^7;

float GetTemperature(void);
void ReadRomCode(unsigned char * rom_code_bufff);


#endif
