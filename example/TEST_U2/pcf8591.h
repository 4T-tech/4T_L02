#ifndef __PCF8591_H
#define __PCF8591_H

#include "iic.h"

void InitPCF8591(void);
void SetDAC(unsigned char dat);

unsigned char GetADC(unsigned char channel);


#endif