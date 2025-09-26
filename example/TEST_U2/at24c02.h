#ifndef __AT24C02_H
#define __AT24C02_H

#include "iic.h"

void write_AT24C02(unsigned char addr, unsigned char dat);
unsigned char read_AT24C02(unsigned char addr);
#endif