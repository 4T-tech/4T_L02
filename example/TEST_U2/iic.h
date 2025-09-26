#ifndef __IIC_H
#define __IIC_H

#include "stc12.h"
#include "intrins.h"

#define uchar unsigned char
#define uint unsigned  int
#define AT24C02_ADDR  0xa0	//AT24C02µÿ÷∑

sbit scl = P2^1;
sbit sda = P2^2;




void I2C_init();
void I2C_Start();
void I2C_Stop();
void Master_ACK(bit i);
void I2C_send_byte(uchar byte);
uchar I2C_read_byte();
bit Test_ACK();
bit I2C_TransmitData(uchar ADDR, DAT);
uchar I2C_ReceiveData(uchar ADDR);
#endif