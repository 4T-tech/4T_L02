#include "pcf8591.h"

void Delay10ms(void)	//@11.0592MHz
{
	unsigned char data i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void InitPCF8591(void)
{
	I2C_Start();
	I2C_send_byte(0x90);
	Test_ACK();
	I2C_send_byte(0x43);  //
	Test_ACK();
	I2C_Stop();
}

//
unsigned char GetADC(unsigned char channel)
{
	unsigned char temp;
	I2C_Start();
	I2C_send_byte(0x90);
	Test_ACK();
	I2C_send_byte(0x40 | channel);  //
	Test_ACK();
	//Delay10Ms();
    
	I2C_Start();
	I2C_send_byte(0x91);
	Test_ACK();
	temp = I2C_read_byte();
	Master_ACK(1);
	I2C_Stop();
	
	return temp;
}

/**/
void SetDAC(unsigned char dat)
{
	I2C_Start();
	I2C_send_byte(0x90);
	Test_ACK();
	I2C_send_byte(0x40);  //
	Test_ACK();
	
	I2C_send_byte(dat);
	Test_ACK();
	I2C_Stop();
}
